/**
*
* Copyright 2014 Future TV, Inc.
*
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the License). You may not use this file except in
 * compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.icntv.tv/licenses/LICENSE-1.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
*
* @author   li.he@icntv.tv
* @date     2014-10-10
*/
#include "URI.h"
#include "log.h"
#include "misc.h"

using namespace std;

URI::URI()
        : Poco::URI() {
}

URI::URI(const std::string& uri)
        : Poco::URI(uri) {
}

void URI::setSignature(const string token, const string encryption)
{
	string query = getQuery();

	setSignature(token, encryption, query);

	setQuery(query);
}

void URI::setSignature(const string token, const string encryption, string &data)
{
	LOG(DEBUG) << "setSignature : data : " << data;

	// sort args and add token for encryption
	list<string> arglist = split(data, "&");
	arglist.sort();
	string sortStr = "";
	for (list<string>::iterator it = arglist.begin(); it != arglist.end(); it++)
	{
		sortStr.append(*it);
	}
	sortStr.append(token);

	// encryption
	icntvEncrypt mIcntvEncrypt;
	string signature = mIcntvEncrypt.md5Encrypt(sortStr.c_str());

	data.append("&sign="+signature);
}
