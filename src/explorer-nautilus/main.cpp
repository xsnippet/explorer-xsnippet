/*!
 * explorer-extension
 * Copyright (c) 2012 Igor Kalnitsky <igor@kalnitsky.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string>

#include <windows.h>
#include <curl/curl.h>


std::string sendFile(const char* filename);
size_t headerHandler(void* response, size_t size, size_t nmemb, void* userdata);
void copyToClipboard(const std::string& text);
std::string getUserName();
std::string unquote(const char* filename);


int APIENTRY WinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                     char* lpCmdLine, int /*nCmdShow*/)
{
    std::string filename = unquote(lpCmdLine);
    std::string snippetUrl = sendFile(filename.c_str());
    copyToClipboard(snippetUrl);
    return 0;
}

/**
 * Send file to xsnippet.org
 *
 * @param filename -- a path to file
 * @return a snippet url 
 */
std::string sendFile(const char* filename)
{
    const char* API_URL = "http://www.xsnippet.org/new";
    std::string snippetUrl = "";

    curl_global_init(CURL_GLOBAL_ALL);

    // set form fields
    curl_httppost* formpost = 0;
    curl_httppost* lastptr = 0;

    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "file",
        CURLFORM_FILE, filename, CURLFORM_END);

    curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "author",
        CURLFORM_COPYCONTENTS, getUserName().c_str(), CURLFORM_END);

    // send data, return url and clean-up
    CURL* curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerHandler);
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, &snippetUrl);
        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }

    return snippetUrl;
}

/**
 * Find snippet url and return last one via userdata.
 *
 * @param response -- a header string
 * @param userdata -- a pointer to std::string
 *
 * @note Call for each header string. 
 */
size_t headerHandler(void* response, size_t size, size_t nmemb, void* userdata)
{
    const std::string LOCATION = "Location";
    std::string header(static_cast<char*>(response));

    if (header.compare(0, LOCATION.length(), LOCATION) == 0)
    {
        size_t found = header.find("http://");
        if (found != std::string::npos)
        {
            std::string* url = static_cast<std::string*>(userdata);
            *url = header.substr(found);
        }
    }

    return size * nmemb;
}

/**
 * Copy text to clipboard.
 *
 * @param text -- an ascii encoded text
 */
void copyToClipboard(const std::string& text)
{
    HGLOBAL hMemory = GlobalAlloc(GMEM_MOVEABLE, text.length() + 1);
    memcpy(GlobalLock(hMemory), text.c_str(), text.length());
    GlobalUnlock(hMemory);

    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMemory);
    CloseClipboard();
}

/**
 * Retrieve name of current user.
 *
 * @return an username
 */
std::string getUserName()
{
    char username[20];
    DWORD size = sizeof(username);

    GetUserName(username, &size);
    return std::string(username, size);
}

/**
 * Remove quotes around string.
 *
 * @return unquoted string
 */
std::string unquote(const char* str)
{
    if (str[0] == '"' && str[strlen(str)-1] == '"')
        return std::string(str + 1, str + strlen(str) - 1);
    return std::string(str);
}