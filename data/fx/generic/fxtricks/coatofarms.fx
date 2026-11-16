
FxInfo


##################################




Condition
	On 	Time
	Time 	0


	Event
		EName	uarmL1pos
		type	local
		at	col_L
		bhvroverride
			positionoffset -.3 -.5 -.5		
		end
	end
	Event
		EName	uarmL1aim
		type	local
		at	uarmL
		
		bhvroverride
			positionoffset -100 0 -20
			
		end
	end
	Event
		EName	uarmL1x
		type	positonly
		at	uarmL1pos
		geom titanbit_Uarm
		bhvroverride
			scale	.5 .5 1.5
		end
		lookat uarmL1aim
	end
	Event
		EName	LarmL1pos
		type	local
		at	uarmL1x
		bhvroverride
			positionoffset 0 0 1			
		end
	end
	Event
		EName	LarmL1aim
		type	local
		at	LarmL
		
		bhvroverride
			positionoffset -100 0 -20
			
		end
	end
	Event
		EName	LarmL1x
		type	positonly
		at	LarmL1pos
		geom titanbit_Larm
		bhvroverride
			scale	.5 .5 1.5
		end
		lookat LarmL1aim
	end
##

	Event
		EName	uarmR1pos
		type	local
		at	col_R
		bhvroverride
			positionoffset .3 -.5 -.5		
		end
	end
	Event
		EName	uarmR1aim
		type	local
		at	uarmR
		
		bhvroverride
			positionoffset 100 0 -20
			
		end
	end
	Event
		EName	uarmR1x
		type	positonly
		at	uarmR1pos
		geom titanbit_Uarm
		bhvroverride
			scale	.5 .5 1.5
		end
		lookat uarmR1aim
	end
	Event
		EName	LarmR1pos
		type	local
		at	uarmR1x
		bhvroverride
			positionoffset 0 0 1			
		end
	end
	Event
		EName	LarmR1aim
		type	local
		at	LarmR
		
		bhvroverride
			positionoffset 100 0 -20
			
		end
	end
	Event
		EName	LarmR1x
		type	positonly
		at	LarmR1pos
		geom titanbit_Larm
		bhvroverride
			scale	.5 .5 1.5
		end
		lookat LarmR1aim
	end
end


Condition
	On 	Time
	Time 	0

	Event
		EName	uarmL2pos
		type	local
		at	col_L
		bhvroverride
			positionoffset -.3 .5 -.5		
		end
	end
	Event
		EName	uarmL2aim
		type	local
		at	uarmL
		
		bhvroverride
			positionoffset -100 40 -20
			
		end
	end
	Event
		EName	uarmL2x
		type	positonly
		at	uarmL2pos
		geom titanbit_Uarm
		bhvroverride
			scale	.5 .5 1.5
		end
		lookat uarmL2aim
	end
	Event
		EName	LarmL2pos
		type	local
		at	uarmL2x
		bhvroverride
			positionoffset 0 0 1			
		end
	end
	Event
		EName	LarmL2aim
		type	local
		at	LarmL
		
		bhvroverride
			positionoffset -100 30 -20
			
		end
	end
	Event
		EName	LarmL2x
		type	positonly
		at	LarmL2pos
		geom titanbit_Larm
		bhvroverride
			scale	.5 .5 1.5
		end
		lookat LarmL2aim
	end
##

	Event
		EName	uarmR2pos
		type	local
		at	col_R
		bhvroverride
			positionoffset .3 .5 -.5		
		end
	end
	Event
		EName	uarmR2aim
		type	local
		at	uarmR
		
		bhvroverride
			positionoffset 100 40 -20
			
		end
	end
	Event
		EName	uarmR2x
		type	positonly
		at	uarmR2pos
		geom titanbit_Uarm
		bhvroverride
			scale	.5 .5 1.5
		end
		lookat uarmR2aim
	end
	Event
		EName	LarmR2pos
		type	local
		at	uarmR2x
		bhvroverride
			positionoffset 0 0 1		
		end
	end
	Event
		EName	LarmR2aim
		type	local
		at	LarmR
		
		bhvroverride
			positionoffset 100 30 -20
			
		end
	end
	Event
		EName	LarmR2x
		type	positonly
		at	LarmR2pos
		geom titanbit_Larm
		bhvroverride
			scale	.5 .5 1.5
		end
		lookat LarmR2aim
	end
end
