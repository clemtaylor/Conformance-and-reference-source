/**
	Copyright (c) 2013- Vlad Zbarsky <zbarsky@cornell.edu>
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.
	* Neither the name of the <organization> nor the
	names of its contributors may be used to endorse or promote products
	derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR COmpeg transport streamNSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	@author	Vlad Zbarsky <zbarsky@cornell.edu>
	@since	Jan 14, 2013
*/
package org.iso.mpeg.dash.crypto;

/**
 * @author Vlad Zbarsky <zbarsky@cornell.edu>
 *
 */
public class DashCryptoExceptionUnsupported extends DashCryptoException {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4455909060325701061L;

	/**
	 * 
	 */
	public DashCryptoExceptionUnsupported() {
		super();
	}

	/**
	 * @param message
	 */
	public DashCryptoExceptionUnsupported(String message) {
		super(message);
	}

	/**
	 * @param cause
	 */
	public DashCryptoExceptionUnsupported(Throwable cause) {
		super(cause);
	}

	/**
	 * @param message
	 * @param cause
	 */
	public DashCryptoExceptionUnsupported(String message, Throwable cause) {
		super(message, cause);
	}
}
