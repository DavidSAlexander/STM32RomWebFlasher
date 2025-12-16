/********************************************************************************************************
 *  [FILE NAME]   :      <STM32WebPages.cpp>                                                            *
 *  [AUTHOR]      :      <David S. Alexander>                                                           *
 *  [DATE CREATED]:      <Dec 10, 2025>                                                                 *
 *  [Description] :      <Source file for STM32 Web Pages>                                              *
 ********************************************************************************************************/


#include "STM32WebPages.h"

const char LOGIN_HTML_A[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>STM32 Bootloader Login</title>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
<style>
:root {
	--primary: #2c3e50;
	--secondary: #3498db;
	--accent: #2ecc71;
	--danger: #e74c3c;
	--light: #ecf0f1;
	--card-bg: #ffffff;
	--shadow: rgba(0, 0, 0, 0.12);
}

* {
	margin: 0;
	padding: 0;
	box-sizing: border-box;
}

body {
	font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
	background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
	min-height: 100vh;
	display: flex;
	align-items: center;
	justify-content: center;
	padding: 20px;
	color: #333;
}

.login-card {
	background: var(--card-bg);
	border-radius: 16px;
	padding: 30px 28px;
	max-width: 380px;
	width: 100%;
	box-shadow: 0 14px 40px var(--shadow);
}

.login-header {
	text-align: center;
	margin-bottom: 20px;
}

.login-header h1 {
	font-size: 22px;
	color: var(--primary);
	margin-bottom: 6px;
	display: flex;
	align-items: center;
	justify-content: center;
	gap: 8px;
}

.login-header p {
	font-size: 13px;
	color: #777;
}

.login-header i {
	color: var(--secondary);
}

.field-group {
	margin-bottom: 15px;
}

label {
	display: block;
	font-size: 13px;
	color: #555;
	margin-bottom: 5px;
	font-weight: 600;
}

input[type="text"],
input[type="password"] {
	width: 100%;
	padding: 10px 12px;
	border-radius: 8px;
	border: 1px solid #ccc;
	font-size: 14px;
	outline: none;
	transition: border 0.2s ease, box-shadow 0.2s ease;
}

input[type="text"]:focus,
input[type="password"]:focus {
	border-color: var(--secondary);
	box-shadow: 0 0 0 2px rgba(52, 152, 219, 0.15);
}

.btn {
	width: 100%;
	padding: 11px 16px;
	border-radius: 10px;
	border: none;
	font-size: 14px;
	font-weight: 600;
	cursor: pointer;
	display: inline-flex;
	align-items: center;
	justify-content: center;
	gap: 8px;
	color: #fff;
	background: var(--secondary);
	transition: background 0.2s, transform 0.15s, box-shadow 0.15s;
}

.btn:hover {
	background: #2980b9;
	transform: translateY(-1px);
	box-shadow: 0 7px 18px rgba(52, 152, 219, 0.3);
}

.btn:disabled {
	opacity: 0.65;
	cursor: not-allowed;
	box-shadow: none;
	transform: none;
}

.status {
	margin-top: 10px;
	font-size: 13px;
	min-height: 18px;
	text-align: center;
}

.status.ok {
	color: var(--accent);
}

.status.err {
	color: var(--danger);
}

.footer {
	margin-top: 16px;
	font-size: 11px;
	text-align: center;
	color: #999;
}

.footer code {
	background: #f4f5f7;
	padding: 2px 4px;
	border-radius: 4px;
	font-size: 11px;
}
</style>
</head>
<body>
<div class="login-card">
<div class="login-header">
<h1><i class="fas fa-microchip"></i> STM32 Bootloader</h1>
<p>Sign in to access the FOTA control panel</p>
</div>

<form id="loginForm">
<div class="field-group">
<label for="user">Username</label>
<input type="text" id="user" name="user" autocomplete="username" required>
</div>

<div class="field-group">
<label for="pass">Password</label>
<input type="password" id="pass" name="pass" autocomplete="current-password" required>
</div>

<button type="submit" id="loginBtn" class="btn">
<i class="fas fa-sign-in-alt"></i> Login
</button>
</form>

<div id="statusMsg" class="status"></div>

<div class="footer">
<p>Device host: <code>http://)rawliteral";

const char LOGIN_HTML_B[] PROGMEM = R"rawliteral(.local/</code></p>
<p>By David S. Alexander</p>
</div>
</div>

<script>
const form = document.getElementById('loginForm');
const btn  = document.getElementById('loginBtn');
const msg  = document.getElementById('statusMsg');

form.addEventListener('submit', async (e) => {
	e.preventDefault();
	msg.textContent = '';
	msg.className = 'status';

	const u = document.getElementById('user').value.trim();
	const p = document.getElementById('pass').value;

	if (!u || !p) {
		msg.textContent = 'Please enter username and password.';
		msg.classList.add('err');
		return;
	}

	btn.disabled = true;
	const originalHtml = btn.innerHTML;
	btn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Checking...';

	try {
		const body = 'user=' + encodeURIComponent(u) + '&pass=' + encodeURIComponent(p);
		const res = await fetch('/login', {
			method: 'POST',
			headers: {'Content-Type': 'application/x-www-form-urlencoded'},
			body
		});
		const data = await res.json();
		if (data.ok) {
			msg.textContent = 'Login successful. Redirecting...';
			msg.classList.add('ok');
			setTimeout(() => { window.location.href = '/'; }, 600);
			} else {
			msg.textContent = data.error || 'Invalid username or password.';
			msg.classList.add('err');
		}
		} catch (err) {
		msg.textContent = 'Login error: ' + err.message;
		msg.classList.add('err');
		} finally {
		btn.disabled = false;
		btn.innerHTML = originalHtml;
	}
});
</script>
</body>
</html>
)rawliteral";

const char INDEX_HTML_A[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>STM32 FOTA Control Panel</title>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
<style>
:root {
	--primary: #2c3e50;
	--secondary: #3498db;
	--accent: #2ecc71;
	--danger: #e74c3c;
	--warning: #f39c12;
	--light: #ecf0f1;
	--dark: #2c3e50;
	--card-bg: #ffffff;
	--shadow: rgba(0, 0, 0, 0.08);
}

* {
	margin: 0;
	padding: 0;
	box-sizing: border-box;
}

body {
	font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
	background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
	color: #333;
	line-height: 1.6;
	min-height: 100vh;
	padding: 20px;
}

.container {
	max-width: 1000px;
	margin: 0 auto;
}

header {
	background: var(--card-bg);
	border-radius: 15px;
	padding: 25px 30px;
	margin-bottom: 25px;
	box-shadow: 0 10px 30px var(--shadow);
	border-left: 5px solid var(--secondary);
}

h1 {
	color: var(--primary);
	font-size: 28px;
	margin-bottom: 8px;
	display: flex;
	align-items: center;
	gap: 12px;
}

h1 i {
	color: var(--secondary);
}

.subtitle {
	color: #666;
	font-size: 16px;
	margin-bottom: 15px;
}

.header-row {
	display: flex;
	flex-wrap: wrap;
	align-items: center;
	justify-content: space-between;
	gap: 12px;
}

.header-actions {
	display: flex;
	gap: 8px;
	align-items: center;
	flex-wrap: wrap;
}

.info-box {
	background: #f8f9fa;
	border-radius: 10px;
	padding: 15px;
	margin-top: 15px;
	border-left: 4px solid var(--warning);
}

.target-info {
	font-size: 14px;
	color: #444;
	margin-top: 8px;
}

.target-info span {
	font-weight: 600;
	color: var(--secondary);
}

.card {
	background: var(--card-bg);
	border-radius: 15px;
	padding: 25px;
	margin-bottom: 25px;
	box-shadow: 0 8px 25px var(--shadow);
	transition: transform 0.3s ease, box-shadow 0.3s ease;
}

.card:hover {
	transform: translateY(-5px);
	box-shadow: 0 15px 35px rgba(0, 0, 0, 0.1);
}

.card-title {
	color: var(--primary);
	font-size: 20px;
	margin-bottom: 20px;
	display: flex;
	align-items: center;
	gap: 10px;
}

.card-title i {
	color: var(--secondary);
}

.upload-area {
	border: 3px dashed #ddd;
	border-radius: 10px;
	padding: 40px 20px;
	text-align: center;
	margin-bottom: 20px;
	transition: all 0.3s;
	background: #fafafa;
}

.upload-area:hover, .upload-area.dragover {
	border-color: var(--secondary);
	background: #f0f8ff;
}

.upload-icon {
	font-size: 48px;
	color: var(--secondary);
	margin-bottom: 15px;
}

.file-input-wrapper {
	position: relative;
	display: inline-block;
	margin-top: 15px;
}

.file-input-wrapper input[type="file"] {
	position: absolute;
	left: 0;
	top: 0;
	opacity: 0;
	width: 100%;
	height: 100%;
	cursor: pointer;
}

.file-btn {
	background: var(--secondary);
	color: white;
	border: none;
	padding: 12px 25px;
	border-radius: 8px;
	font-weight: 600;
	cursor: pointer;
	display: inline-flex;
	align-items: center;
	gap: 8px;
	transition: all 0.3s;
}

.file-btn:hover {
	background: #2980b9;
	transform: scale(1.05);
}

.btn {
	padding: 10px 18px;
	border: none;
	border-radius: 10px;
	font-weight: 600;
	cursor: pointer;
	transition: all 0.3s;
	display: inline-flex;
	align-items: center;
	justify-content: center;
	gap: 8px;
	font-size: 14px;
}

.btn-primary {
	background: var(--secondary);
	color: white;
}

.btn-primary:hover {
	background: #2980b9;
	transform: translateY(-2px);
	box-shadow: 0 4px 12px rgba(52, 152, 219, 0.3);
}

.btn-danger {
	background: var(--danger);
	color: white;
}

.btn-danger:hover {
	background: #c0392b;
	transform: translateY(-2px);
	box-shadow: 0 4px 12px rgba(231, 76, 60, 0.3);
}

.btn-success {
	background: var(--accent);
	color: white;
}

.btn-success:hover {
	background: #27ae60;
	transform: translateY(-2px);
	box-shadow: 0 4px 12px rgba(46, 204, 113, 0.3);
}

.btn-warning {
	background: var(--warning);
	color: white;
}

.btn-warning:hover {
	background: #e67e22;
	transform: translateY(-2px);
	box-shadow: 0 4px 12px rgba(243, 156, 18, 0.3);
}

.btn-info {
	background: #9b59b6;
	color: white;
}

.btn-info:hover {
	background: #8e44ad;
	transform: translateY(-2px);
	box-shadow: 0 4px 12px rgba(155, 89, 182, 0.3);
}

.btn:disabled {
	opacity: 0.6;
	cursor: not-allowed;
	transform: none !important;
	box-shadow: none !important;
}

.button-grid {
	display: grid;
	grid-template-columns: repeat(auto-fill, minmax(220px, 1fr));
	gap: 15px;
	margin-top: 10px;
}

.log-container {
	background: #1a1a1a;
	border-radius: 10px;
	padding: 20px;
	margin-top: 10px;
	overflow: hidden;
}

#log {
	background: #1a1a1a;
	color: #00ff00;
	font-family: 'Consolas', 'Monaco', monospace;
	min-height: 200px;
	max-height: 400px;
	overflow-y: auto;
	padding: 15px;
	border-radius: 8px;
	white-space: pre-wrap;
	word-break: break-all;
	font-size: 14px;
	border: 1px solid #333;
}

.log-header {
	display: flex;
	justify-content: space-between;
	align-items: center;
	margin-bottom: 15px;
}

.log-controls button {
	background: #444;
	color: white;
	border: none;
	padding: 6px 12px;
	border-radius: 5px;
	font-size: 13px;
	cursor: pointer;
	margin-left: 8px;
}

.log-entry {
	padding: 5px 0;
	border-bottom: 1px solid #2a2a2a;
}

.log-time {
	color: #888;
}

.log-cmd {
	color: #3498db;
	font-weight: bold;
}

.log-response {
	color: #2ecc71;
}

.log-error {
	color: #e74c3c;
}

.status-indicator {
	display: inline-flex;
	align-items: center;
	gap: 8px;
	padding: 6px 12px;
	border-radius: 20px;
	background: #f0f0f0;
	font-size: 13px;
}

.status-dot {
	width: 10px;
	height: 10px;
	border-radius: 50%;
	background: #95a5a6;
}

.status-dot.active {
	background: var(--accent);
	animation: pulse 1.5s infinite;
}

@keyframes pulse {
	0% { opacity: 1; }
	50% { opacity: 0.5; }
	100% { opacity: 1; }
}

.progress-container {
	margin-top: 15px;
	display: none;
}

.progress-bar {
	height: 10px;
	background: #eee;
	border-radius: 5px;
	overflow: hidden;
	margin-top: 10px;
}

.progress-fill {
	height: 100%;
	background: linear-gradient(90deg, var(--secondary), var(--accent));
	width: 0%;
	transition: width 0.3s;
}

.legend-wrapper {
	margin-top: 18px;
	font-size: 13px;
	color: #666;
}

.legend-badge {
	display: inline-block;
	padding: 4px 10px;
	border-radius: 12px;
	margin-right: 8px;
	font-size: 12px;
	font-weight: 600;
	cursor: default;
}

.legend-full     { background: rgba(46, 204, 113, 0.12); color: #27ae60; }
.legend-destr    { background: rgba(243, 156, 18, 0.12); color: #e67e22; }
.legend-prog     { background: rgba(52, 152, 219, 0.12); color: #2980b9; }
.legend-info     { background: rgba(155, 89, 182, 0.12); color: #8e44ad; }

.footer {
	text-align: center;
	margin-top: 30px;
	color: rgba(255, 255, 255, 0.7);
	font-size: 14px;
}

@media (max-width: 768px) {
	.card {
		padding: 20px;
	}
	h1 {
		font-size: 24px;
	}
	.header-row {
		flex-direction: column;
		align-items: flex-start;
		gap: 10px;
	}
	.header-actions {
		width: 100%;
		justify-content: flex-start;
	}
}
</style>
</head>
<body>
<div class="container">
<header>
<h1><i class="fas fa-microchip"></i> STM32 FOTA Control Panel</h1>
<p class="subtitle">ESP8266-based Firmware Update System using STM32 ROM Bootloader</p>

<div class="header-row">
<div>
<div class="target-info">
<strong>Target:</strong> <span id="targetInfo">Not connected</span>
</div>
<div class="status-indicator" style="margin-top: 10px;">
<div class="status-dot active"></div>
<span>ESP8266 Online</span>
</div>
</div>

<div class="header-actions">
<button id="connectBtn" class="btn btn-primary">
<i class="fas fa-plug"></i> Connect Target
</button>
<button id="logoutBtn" class="btn" style="background:#7f8c8d;color:#fff;">
<i class="fas fa-sign-out-alt"></i> Logout
</button>
</div>
</div>

<div class="info-box">
<p><strong>Workflow:</strong> Connect target → Upload <code>update.bin</code> → Run bootloader commands.</p>
<p><strong>Current file:</strong> <span id="currentFile">None</span></p>
</div>
</header>

<div class="card" id="uploadCard" style="display:none;">
<h2 class="card-title"><i class="fas fa-upload"></i> 1. Upload Firmware</h2>
<p>Select a binary (.bin) file to upload to ESP8266 filesystem (saved as /update.bin)</p>

<div class="upload-area" id="dropArea">
<div class="upload-icon">
<i class="fas fa-cloud-upload-alt"></i>
</div>
<p><strong>Drag & drop your firmware file here</strong></p>
<p>or</p>
<div class="file-input-wrapper">
<button class="file-btn">
<i class="fas fa-folder-open"></i> Browse Files
</button>
<form id="uploadForm" method="POST" action="/upload" enctype="multipart/form-data">
<input type="file" name="firmware" id="fwFile" accept=".bin" required>
</form>
</div>
<p class="file-hint">Maximum file size limited by ESP8266 FS (LittleFS).</p>
</div>

<div class="progress-container" id="progressContainer">
<div>Uploading: <span id="progressText">0%</span></div>
<div class="progress-bar">
<div class="progress-fill" id="progressFill"></div>
</div>
</div>

<div id="uploadStatus" style="margin-top: 15px; font-weight: 600;"></div>

<button id="uploadBtn" class="btn btn-primary" style="margin-top: 15px; width: 100%;">
<i class="fas fa-upload"></i> Upload Firmware
</button>
</div>

<div class="card" id="cmdCard" style="display:none;">
<h2 class="card-title"><i class="fas fa-terminal"></i> 2. Bootloader Commands</h2>
<p>Control the STM32 built-in ROM bootloader via ESP8266</p>

<div class="button-grid">
<button class="btn btn-success" data-cmd="S">
<i class="fas fa-play-circle"></i> Full Update
</button>

<button class="btn btn-warning" data-cmd="E">
<i class="fas fa-eraser"></i> Erase Only
</button>

<button class="btn btn-primary" data-cmd="U">
<i class="fas fa-code"></i> Program Only
</button>

<button class="btn btn-info" data-cmd="J">
<i class="fas fa-redo"></i> Reset to App
</button>

<button class="btn btn-info" data-cmd="G">
<i class="fas fa-id-card"></i> Read Chip ID
</button>

<button class="btn btn-info" data-cmd="R">
<i class="fas fa-info-circle"></i> Bootloader Version
</button>

<button class="btn btn-info" data-cmd="C">
<i class="fas fa-list"></i> Get Commands
</button>

<button class="btn btn-danger" data-cmd="T">
<i class="fas fa-vial"></i> Test RAM Write
</button>
</div>

<div class="legend-wrapper">
<strong>Legend:</strong>
<span class="legend-badge legend-full">Full Process</span>
<span class="legend-badge legend-destr">Destructive</span>
<span class="legend-badge legend-prog">Programming</span>
<span class="legend-badge legend-info">Info / Utility</span>
</div>
</div>

<div class="card">
<div class="log-header">
<h2 class="card-title"><i class="fas fa-list-alt"></i> Command Log</h2>
<div class="log-controls">
<button onclick="clearLog()"><i class="fas fa-trash"></i> Clear</button>
<button onclick="toggleAutoScroll()" id="scrollToggle"><i class="fas fa-arrow-down"></i> Auto-scroll ON</button>
</div>
</div>

<div class="log-container">
<div id="log"></div>
</div>

<div style="margin-top: 15px; display: flex; gap: 10px; flex-wrap: wrap;">
<button class="btn" onclick="exportLog()" style="background: #7f8c8d; color: white;">
<i class="fas fa-download"></i> Export Log
</button>
</div>
</div>

<div class="footer">
<p>STM32 FOTA Panel | ESP8266 Web Interface | ROM Bootloader</p>
<p>Access via mDNS: <code>http://)rawliteral";

const char INDEX_HTML_B[] PROGMEM = R"rawliteral(.local/</code></p>
<p>By David S. Alexander</p>
</div>
</div>

<script>
const logDiv = document.getElementById('log');
const uploadBtn = document.getElementById('uploadBtn');
const uploadForm = document.getElementById('uploadForm');
const fwFileInput = document.getElementById('fwFile');
const uploadStatus = document.getElementById('uploadStatus');
const dropArea = document.getElementById('dropArea');
const progressContainer = document.getElementById('progressContainer');
const progressFill = document.getElementById('progressFill');
const progressText = document.getElementById('progressText');
const currentFileSpan = document.getElementById('currentFile');
const connectBtn = document.getElementById('connectBtn');
const logoutBtn = document.getElementById('logoutBtn');
const targetInfoSpan = document.getElementById('targetInfo');
const uploadCard = document.getElementById('uploadCard');
const cmdCard = document.getElementById('cmdCard');
let autoScroll = true;
let isConnected = false;

addLog('System initialized. Ready for connect.', 'system');

['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
	dropArea.addEventListener(eventName, e => {
		e.preventDefault();
		e.stopPropagation();
	}, false);
});

['dragenter', 'dragover'].forEach(eventName => {
	dropArea.addEventListener(eventName, () => dropArea.classList.add('dragover'), false);
});

['dragleave', 'drop'].forEach(eventName => {
	dropArea.addEventListener(eventName, () => dropArea.classList.remove('dragover'), false);
});

dropArea.addEventListener('drop', e => {
	const dt = e.dataTransfer;
	const files = dt.files;
	if (files.length) {
		fwFileInput.files = files;
		updateFileInfo(files[0]);
		showUploadStatus('File selected: ' + files[0].name, 'info');
	}
}, false);

fwFileInput.addEventListener('change', function() {
	if (this.files.length) updateFileInfo(this.files[0]);
});

function updateFileInfo(file) {
	currentFileSpan.textContent = file.name + ' (' + formatFileSize(file.size) + ')';
	currentFileSpan.style.color = '#2ecc71';
	currentFileSpan.style.fontWeight = 'bold';
}

function formatFileSize(bytes) {
	if (bytes === 0) return '0 Bytes';
	const k = 1024;
	const sizes = ['Bytes', 'KB', 'MB', 'GB'];
	const i = Math.floor(Math.log(bytes) / Math.log(k));
	return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
}

uploadBtn.addEventListener('click', function() {
	if (!isConnected) {
		showUploadStatus('Connect target first.', 'error');
		return;
	}
	if (!fwFileInput.files.length) {
		showUploadStatus('Please select a firmware file first.', 'error');
		return;
	}
	const file = fwFileInput.files[0];
	if (!file.name.endsWith('.bin')) {
		showUploadStatus('Please select a .bin file.', 'error');
		return;
	}
	uploadFile(file);
});

function uploadFile(file) {
	const formData = new FormData();
	formData.append('firmware', file);

	uploadBtn.disabled = true;
	uploadBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Uploading...';
	progressContainer.style.display = 'block';
	progressFill.style.width = '0%';
	progressText.textContent = '0%';

	const xhr = new XMLHttpRequest();

	xhr.upload.addEventListener('progress', function(e) {
		if (e.lengthComputable) {
			const percentComplete = (e.loaded / e.total) * 100;
			progressFill.style.width = percentComplete + '%';
			progressText.textContent = Math.round(percentComplete) + '%';
		}
	});

	xhr.addEventListener('load', function() {
		uploadBtn.disabled = false;
		uploadBtn.innerHTML = '<i class="fas fa-upload"></i> Upload Firmware';

		if (xhr.status === 200) {
			showUploadStatus('Upload successful: ' + xhr.responseText, 'success');
			addLog('Firmware uploaded: ' + file.name + ' (' + formatFileSize(file.size) + ')', 'upload');
			} else {
			showUploadStatus('Upload failed: ' + xhr.statusText, 'error');
		}

		setTimeout(() => { progressContainer.style.display = 'none'; }, 2000);
	});

	xhr.addEventListener('error', function() {
		uploadBtn.disabled = false;
		uploadBtn.innerHTML = '<i class="fas fa-upload"></i> Upload Firmware';
		showUploadStatus('Upload error. Check connection.', 'error');
		progressContainer.style.display = 'none';
	});

	xhr.open('POST', '/upload');
	xhr.send(formData);
}

function showUploadStatus(message, type) {
	uploadStatus.textContent = message;
	uploadStatus.style.color =
	type === 'success' ? '#2ecc71' :
	type === 'error'   ? '#e74c3c' :
	type === 'info'    ? '#3498db' : '#333';
}

connectBtn.addEventListener('click', async () => {
	connectBtn.disabled = true;
	try {
		if (!isConnected) {
			connectBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Connecting...';
			const res = await fetch('/connect', { method: 'POST' });
			const data = await res.json();
			if (data.ok) {
				addLog('Connected: ' + (data.desc || ''), 'response');
				} else {
				alert('Connect failed: ' + (data.error || 'unknown error'));
			}
			await refreshStatus();
			} else {
			connectBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Disconnecting...';
			const res = await fetch('/disconnect', { method: 'POST' });
			await res.json().catch(() => {});
			addLog('Target disconnected', 'system');
			await refreshStatus();
		}
		} catch (err) {
		alert('Connect/disconnect error: ' + err.message);
		} finally {
		connectBtn.disabled = false;
	}
});

logoutBtn.addEventListener('click', async () => {
	if (!confirm('Logout and require login again on all devices?')) return;
	logoutBtn.disabled = true;
	const original = logoutBtn.innerHTML;
	logoutBtn.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Logging out...';
	try {
		const res = await fetch('/logout', { method: 'POST' });
		await res.json().catch(() => {});
		window.location.href = '/';
		} catch (err) {
		alert('Logout failed: ' + err.message);
		logoutBtn.disabled = false;
		logoutBtn.innerHTML = original;
	}
});

async function refreshStatus() {
	try {
		const res = await fetch('/status');
		if (!res.ok) return;
		const data = await res.json();
		updateUiForStatus(data);
		} catch (e) {}
	}

	function updateUiForStatus(status) {
		isConnected = !!status.connected;

		if (isConnected) {
			connectBtn.innerHTML = '<i class="fas fa-unlink"></i> Disconnect Target';
			connectBtn.classList.remove('btn-primary');
			connectBtn.classList.add('btn-danger');
			targetInfoSpan.textContent = status.desc || 'Target connected';
			uploadCard.style.display = 'block';
			cmdCard.style.display = 'block';
			} else {
			connectBtn.innerHTML = '<i class="fas fa-plug"></i> Connect Target';
			connectBtn.classList.remove('btn-danger');
			connectBtn.classList.add('btn-primary');
			targetInfoSpan.textContent = 'Not connected';
			uploadCard.style.display = 'none';
			cmdCard.style.display = 'none';
		}

		if (status.hasFile && status.connected) {
			currentFileSpan.textContent = 'update.bin on device (Flash ' + status.flashKB + ' KB, devID 0x0' +
			status.devId.toString(16) + ')';
			currentFileSpan.style.color = '#2ecc71';
			} else if (status.hasFile) {
			currentFileSpan.textContent = 'update.bin on device (connect target to use)';
			currentFileSpan.style.color = '#3498db';
			} else {
			currentFileSpan.textContent = 'None';
			currentFileSpan.style.color = '#444';
		}
	}

	document.querySelectorAll('#cmdCard .btn').forEach(btn => {
		const cmd = btn.getAttribute('data-cmd');
		if (!cmd) return;
		btn.addEventListener('click', () => sendCmd(cmd, btn));
	});

	async function sendCmd(cmd, button) {
		if (!isConnected) {
			addLog('Target not connected. Connect first.', 'error');
			alert('Connect target first.');
			return;
		}

		const cmdNames =
		{
			'S': 'Full Update',
			'E': 'Erase Only',
			'U': 'Program Only',
			'J': 'Jump to Application',
			'G': 'Read Chip ID',
			'R': 'Read Bootloader Version',
			'C': 'Get Supported Commands',
			'T': 'Test RAM Write'
		};

		addLog('Sending: ' + (cmdNames[cmd] || cmd), 'cmd');

		const originalText = button.innerHTML;
		button.disabled = true;
		button.innerHTML = '<i class="fas fa-spinner fa-spin"></i> Processing...';

		try {
			const response = await fetch('/cmd?c=' + encodeURIComponent(cmd));
			const text = await response.text();
			addLog('Response: ' + text, 'response');
			} catch (err) {
			addLog('Error: ' + err.message, 'error');
			} finally {
			button.disabled = false;
			button.innerHTML = originalText;
		}
	}

	function addLog(message, type = 'info') {
		const now = new Date();
		const timeString = now.toLocaleTimeString([], {hour: '2-digit', minute:'2-digit', second:'2-digit'});
		const typeClass = type === 'cmd' ? 'log-cmd' :
		type === 'response' ? 'log-response' :
		type === 'error' ? 'log-error' : '';

		const logEntry = document.createElement('div');
		logEntry.className = 'log-entry';
		logEntry.innerHTML = `<span class="log-time">[${timeString}]</span> <span class="${typeClass}">${message}</span>`;
		logDiv.prepend(logEntry);

		if (logDiv.children.length > 200) {
			logDiv.removeChild(logDiv.lastChild);
		}

		if (autoScroll && logDiv.firstChild) {
			logDiv.scrollTop = 0;
		}
	}

	function clearLog() {
		logDiv.innerHTML = '';
		addLog('Log cleared', 'system');
	}

	function toggleAutoScroll() {
		autoScroll = !autoScroll;
		const toggleBtn = document.getElementById('scrollToggle');
		if (autoScroll) {
			toggleBtn.innerHTML = '<i class="fas fa-arrow-down"></i> Auto-scroll ON';
			toggleBtn.style.background = '#2ecc71';
			} else {
			toggleBtn.innerHTML = '<i class="fas fa-times"></i> Auto-scroll OFF';
			toggleBtn.style.background = '#e74c3c';
		}
	}

	function exportLog() {
		const logContent = Array.from(logDiv.children)
		.map(entry => entry.textContent)
		.reverse()
		.join('\n');

		const blob = new Blob([logContent], {type: 'text/plain'});
		const url = URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		a.download = `fota-log-${new Date().toISOString().slice(0,10)}.txt`;
		document.body.appendChild(a);
		a.click();
		document.body.removeChild(a);
		URL.revokeObjectURL(url);

		addLog('Log exported', 'system');
	}

	document.addEventListener('DOMContentLoaded', () => {
		refreshStatus();
	});
	</script>
	</body>
	</html>
	)rawliteral";
