<h1>Webserv</h1>
<p>This back-end project aims to build a simple web server that allows users to navigate their websites through a browser. The server handles all GET requests on the back-end, delivering accurate results. It supports GET, POST, and DELETE methods and includes a customizable configuration file, enabling users to fine-tune the server to their specific needs. The web server also supports virtual servers, each capable of managing multiple ports, making it easy to handle different websites simultaneously. Built on non-blocking I/O and CGI technologies, the server ensures that no client experiences delays, even while others are uploading files or browsing the site.</p>
<h1>How to Install and Run the Project</h1>
<ol>
   <li><strong>Clone the Repository</strong></li>
   <li><strong>Configure confg.toml file</strong></li>
   <pre><code>path: ./Webserv/conf/confg.toml</code></pre>
   <li><strong>Add the folder's path</strong></li>
   <pre><code>port = [1336, 1338]
	host = 127.0.0.1
	server_name = facebook.com
	root = /Users/hait-hsa/Desktop/Webserv/src <---------here
	index = index.html
	allowed_methods = [GET,POST, DELETE]
	autoindex = on
	_clientMaxBodySize = 1021545510215455

		[[server.location]]
			prefix = /
			autoindex = off
			root = /Users/hait-hsa/Desktop/Webserv/src <---------here
			index = index.html
			error_page = [404, /404.html]
			allowed_methods = [ GET, POST, DELETE]
			_clientMaxBodySize = 1021545510215455
   </code></pre>
   <li><strong>Compile the project</strong></li>
   <pre><code>$ make</code></pre>
   <li><strong>Run the server</strong></li>
   <pre><code>$ ./webserv conf/confg.toml</code>
   <code>
   listening on port 1336 | ip: 127.0.0.1 | SVname: facebook.com
   listening on port 1338 | ip: 127.0.0.1 | SVname: facebook.com
   listening on port 1339 | ip: 127.0.0.1 | SVname: 1.com
   listening on port 2020 | ip: 127.0.0.1 | SVname: 1.com
   </code>
   </pre>
   <p>Great! Now that the server is running, the sockets have been bound correctly and are listening for any incoming requests from the client to handle.</p>
</ol>
<h1>How to Use the Server</h1>
<ol>
   <li><strong>Access the Web Server</strong></li>
   <p>Open your web browser and enter the server’s address:</p>
   <pre><code>http://server-ip:port</code></pre>
   <p>Replace <strong>server-ip</strong> and <strong>port</strong> with the IP address and port specified in the configuration file.</p>
</ol>