/* CONST */
var const_fence_id = 0;

/* AJAX */
function getAJAXData(div_id, request_url, listGroupItem) {
	"use strict";
		
	try {
		
		var insertClass = (listGroupItem) ?
			'list-group-item' : 'panel-body';
		
		$("#" + div_id).fadeIn(
			'<div class="' + insertClass + '">' + 
				'<img src="./static/images/loader.gif" alt="Pending.." />' + 
			'</div>'
		);
		
		var varData, div_class, insertHTML;
		
		var xhttp = new XMLHttpRequest();
		xhttp.onreadystatechange = function() {
			
			if (this.readyState == 4 && this.status == 200) {
				
				$("#resultArray").append(this.responseText);
				
				varData = JSON.parse(this.responseText);
				
				if(varData.status) {
					
					$("#" + div_id).html('');
					
					if(varData.data.length > 0) {
						
						var i = 0;
						varData.data.forEach(function(item) {
							
							div_class = insertClass + ' ' + item.div.class;
							
							insertHTML = ((typeof item.html !== "undefined") && (item.html !== '')) ? 
								'<br />' + item.html : '';
							
							$("#" + div_id).append(
								'<div id="' + item.div.id + '" class="' + div_class + '">' + 
									item.name + 
									'<span class="pull-right gray">' + item.updated + '</span>' +
									insertHTML +
								'</div>'
							);
							
							if(!listGroupItem) {
								
								if((varData.data.length - 1) > i) {
									$("#" + div_id).append('<hr class="ajax-separator" />');
								}

							}
							
							i++;
							
						});

					} else {
						
						$("#" + div_id).html(
							'<div class="' + insertClass + '">' + 
								'no data' + 
							'</div>'
						);

					}
						
					$.notify({
						title: varData.notify.title,
						message: varData.notify.message
					}, {
						type: varData.notify.type
					});
					
				} else {
					
					$("#" + div_id).html(
						'<div class="' + insertClass + ' alert-danger">' + 
							varData.notify.title + 
						'</div>'
					);
					
					$.notify({
						title: varData.notify.title,
						message: varData.notify.message
					}, {
						type: varData.notify.type
					});
					
				}
				
			}
			
		};
		xhttp.open("GET", request_url, true);
		xhttp.send();
		
	} catch(err) {
		
		$.notify({
			title: "Error!",
			message: err.message
		}, {
			type: 'error'
		});
		
	}
	
}
function toggleAJAXData(div_id, request_url) {
	"use strict";
		
	try {
		
		$("#" + div_id).attr('disabled', 'disabled');
		
		var varData;
		
		var xhttp = new XMLHttpRequest();
		xhttp.onreadystatechange = function() {
			
			if (this.readyState == 4 && this.status == 200) {
				
				$("#resultArray").html(this.responseText);
				
				varData = JSON.parse(this.responseText);
							
				$.notify({
					title: varData.notify.title,
					message: varData.notify.message
				}, {
					type: varData.notify.type
				});

				$("#" + div_id).removeAttr('disabled');
				
			}
			
		};
		xhttp.open("GET", request_url, true);
		xhttp.send();
		
	} catch(err) {
		
		$.notify({
			title: "Error!",
			message: err.message
		}, {
			type: 'error'
		});
		
	}
	
}

/* LIVESTOCK */
function setLivestock(fence_id) {
	"use strict";
	
	//
	// Update Livestock List
	
	try {

		if(fence_id < 1) {
			throw("unknown fence id");
		}

		console.log('get fence livestocks: ' + fence_id);
		
		getAJAXData(
			'livestockContainer', 
			'ajax.php?site=livestocks&action=getLivestocks&id=' + fence_id,
			false
		);

	} catch(err) {
		console.log(err.message);
	}	

}

/* GATE */
function setGate(fence_id) {
	"use strict";
	
	//
	// Update Gate List
	
	try {

		if(fence_id < 1) {
			throw("unknown fence id");
		}

		console.log('get fence gates: ' + fence_id);
		
		getAJAXData(
			'gateContainer', 
			'ajax.php?site=gates&action=getGates&id=' + fence_id,
			false
		);

	} catch(err) {
		console.log(err.message);
	}	

}

/* FENCE */
function setFence(fence_id) {
	"use strict";
	
	$("#resultArray").html('');
	
	const_fence_id = fence_id;
	
	if(const_fence_id > 0) {
	
		// Livestocks
		setLivestock(fence_id);

		// Gates
		setGate(fence_id);

	}
	
}
function toggleFenceState(fence_id) {
	"use strict";
	
	toggleAJAXData(
		"toggle-fence-" + fence_id, 
		"ajax.php?site=fences&action=toggleState&id=" + fence_id
	);
	
}

$(document).ready(function() {
	
	setInterval(
		function(){ 

			getAJAXData(
				'fencesContainer', 
				'ajax.php?site=fences&action=getAll',
				false
			);
			
			setFence(const_fence_id);

		}, 
		10000
	);
	
});

/* CONVERT */
function convert2id(id_str) {
	"use strict";
	
	//
	// return: id
	// example: fence-2 > 2
	
	var idArray = id_str.split('-');

	if(idArray.length < 2) {
		throw("wrong id array length: " + idArray.length);
	}

	var id = idArray[idArray.length - 1];
	return id;
	
}