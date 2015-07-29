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
#ifndef ICNTV_URI_H_
#define ICNTV_URI_H_

#include <Poco/URI.h>
#include "icntvEncrypt.h"

class URI : public Poco::URI {
public:
	URI();
    explicit URI(const std::string& uri);

	void setSignature(const std::string token, const std::string encryption);
	void setSignature(const std::string token, const std::string encryption, std::string &data);
};

#endif /* ICNTV_URI_H_ */
