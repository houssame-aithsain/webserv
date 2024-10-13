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
   <li><strong>Make Requests to the Server</strong></li>
   <p>You can interact with the server by making HTTP requests using a browser, <code>curl</code>, or any HTTP client tool (like <code>Postman</code>). Here’s how to use the different HTTP methods:</p>
   <ul>
      <li><strong>GET Request:</strong></li>
      <p>To view a webpage or resource, simply enter the URL in your browser or use:</p>
         <pre><code>curl http://server-ip:port/resource</code></pre>
      <li><strong>POST Request:</strong></li>
      <p>To send data to the server, such as submitting a form or uploading a file:</p>
         <pre><code>curl -X POST -d "key=value" http://server-ip:port/resource</code></pre>
      <li><strong>If uploading a file:</strong></li>
         <pre><code>curl -X POST -F "file=@/path/to/file" http://server-ip:port/resource</code></pre>
      <li><strong>DELETE Request:</strong></li>
      <p>To delete a resource from the server:</p>
         <pre><code>curl -X DELETE http://server-ip:port/resource</code></pre>
   </ul>
</ol>
<h1>Custom Configuration</h1>
<p>The web server supports customizable configuration through a configuration file, allowing users to fine-tune server behavior. Below is an example configuration file with explanations of its parameters.</p>
<p><strong>Example Configuration:</strong></p>
<pre><code>

[[server]]

	port = [1336, 1338]
	host = 127.0.0.1
	server_name = facebook.com
	root = /Users/hait-hsa/Desktop/Webserv/src
	index = index.html
	allowed_methods = [GET,POST, DELETE]
	autoindex = on
	_clientMaxBodySize = 1021545510215455

		[[server.location]]
			prefix = /
			autoindex = off
			root = /Users/hait-hsa/Desktop/Webserv/src
			index = index.html
			error_page = [404, /404.html]
			allowed_methods = [ GET, POST, DELETE]
			_clientMaxBodySize = 1021545510215455

		[[server.location]]
			prefix = /aa
			autoindex = on
			root = /Users/hait-hsa/Desktop/Webserv/src
			index = test.html
			error_page = [404, /404.html]
			allowed_methods = [ GET, POST, DELETE]
			_clientMaxBodySize = 1021545510215455


[[server]]

	port = [ 1339, 2020]
	host = 127.0.0.1
	server_name = 1.com
	root = /Users/hait-hsa/Desktop/Webserv/src
	index = index.html
	allowed_methods = [GET, POST, DELETE]
	error_page = [404, www/error_pages/404.html]
	_clientMaxBodySize = 1021545510215455

		[[server.location]]

			prefix = /aa
			allowed_methods = [GET,POST, DELETE]
			error_page = [404, /404.html]
			cgi = .py

</code></pre>
<ul>
    <li>
        <strong>server:</strong> Defines a web server instance.
    </li>
    <li>
        <strong>port:</strong> A list of ports the server will listen on. For example, [1336, 1338].
    </li>
    <li>
        <strong>host:</strong> The IP address the server binds to (e.g., 127.0.0.1).
    </li>
    <li>
        <strong>server_name:</strong> The name of the server (e.g., facebook.com).
    </li>
    <li>
        <strong>root:</strong> The root directory for serving files.
    </li>
    <li>
        <strong>index:</strong> The default index file (e.g., index.html).
    </li>
    <li>
        <strong>allowed_methods:</strong> Supported HTTP methods like GET, POST, and DELETE.
    </li>
    <li>
        <strong>autoindex:</strong> If set to on, directories without an index file will be listed.
    </li>
    <li>
        <strong>_clientMaxBodySize:</strong> Maximum size for client uploads.
    </li>
    <li>
        <strong>server.location:</strong> Configures specific routes (locations) within the server.
    </li>
    <li>
        <strong>prefix:</strong> The path prefix for this location (e.g., /, /aa, /eee).
    </li>
    <li>
        <strong>autoindex:</strong> Whether to display a directory listing for this location.
    </li>
    <li>
        <strong>root:</strong> The root directory for this location.
    </li>
    <li>
        <strong>index:</strong> The default file to serve when accessing this location.
    </li>
    <li>
        <strong>error_page:</strong> Specifies custom error pages for HTTP status codes (e.g., 404).
    </li>
    <li>
        <strong>cgi:</strong> Defines a CGI script handler (e.g., .py for Python scripts).
    </li>
    <li>
        <strong>upload_path:</strong> Path where uploaded files will be stored (only in certain locations).
    </li>
</ul>
