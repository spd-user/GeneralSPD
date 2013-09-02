function moore(_playerNum){

	var playerNum = _playerNum;
	var size = Math.sqrt(_playerNum) | 0;
	
	if (size * size != playerNum) {
		throw "Non lattice";
	}

	return function(sourceId) {
		var connectedUnits = [];

		var sourceX = (sourceId % size) | 0;
		var sourceY = (sourceId / size) | 0;

		var destId;

		var x, y;
		for (y = -1; y <= 1; y++) {
			for (x = -1; x <= 1; x++) {

				destId = ((sourceX + x + size) % size) + (((sourceY + y + size) % size) * size);
				if (destId > sourceId) {
					connectedUnits.push(destId);
				}
			}
		}
		return connectedUnits;
	};
};
