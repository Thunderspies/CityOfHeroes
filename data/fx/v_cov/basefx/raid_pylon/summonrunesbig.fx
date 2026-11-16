#########################################################
##	forceBubble
########################################################
FxInfo



Flags    InheritAlpha

#################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	Base
		Type	start
		At	origin
		bhvroverride
			Spin			0.0 0.03 0.0
			PositionOffset		0 0 0
		end
	End
end

######### Disks

########## runes

Condition   
	On	time
	Time	0
	chance .15
###
	Event
		EName	o1
		Type	local
		At	Base
		bhvr	:RuneRiseBig.bhvr
		bhvroverride
			PositionOffset		0 0 0
		end
		Lifespan 61
	End
	Event
		EName	R1
		Type	local
		At	o1
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc01RUNE__PylonDisc
		bhvroverride
			PositionOffset		0 -2.274 0
		end
		Lifespan 61
	End
end
###
Condition   
	On	time
	Time	0
	chance .15
	Event
		EName	o2
		Type	local
		At	Base
		bhvr	:RuneRiseBig.bhvr
		bhvroverride
			PositionOffset		-.084 00 0
		end
		Lifespan 61
	End
	Event
		EName	R2
		Type	local
		At	o2
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc02RUNE__PylonDisc
		bhvroverride
			PositionOffset		.084 -3.428 0
		end
		Lifespan 61
	End
end
###
Condition   
	On	time
	Time	0
	chance .15
	Event
		EName	o3
		Type	local
		At	Base
		bhvr	:RuneRiseBig.bhvr
		bhvroverride
			PositionOffset		.067 0 0
		end
		Lifespan 61
	End
	Event
		EName	R3
		Type	local
		At	o3
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc03RUNE__PylonDisc
		bhvroverride
			PositionOffset		-.067 -4.57 0
		end
		Lifespan 61
	End
end
###
Condition   
	On	time
	Time	0
	chance .15
	Event
		EName	o4
		Type	local
		At	Base
		bhvr	:RuneRiseBig.bhvr
		bhvroverride
			PositionOffset		0 0 0
		end
		Lifespan 61
	End
	Event
		EName	R4
		Type	local
		At	o4
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc04RUNE__PylonDisc
		bhvroverride
			PositionOffset		0 -5.598 0
		end
		Lifespan 61
	End
end
###
Condition   
	On	time
	Time	0
	chance .15
	Event
		EName	o5
		Type	local
		At	Base
		bhvr	:RuneRiseBig.bhvr
		bhvroverride
			PositionOffset		-.138 0 0
		end
		Lifespan 61
	End
	Event
		EName	R5
		Type	local
		At	o5
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc05RUNE__PylonDisc
		bhvroverride
			PositionOffset		.138 -6.867 0
		end
		Lifespan 61
	End
end
###
Condition   
	On	time
	Time	0
	chance .15
	Event
		EName	o6
		Type	local
		At	Base
		bhvr	:RuneRiseBig.bhvr
		bhvroverride
			PositionOffset		.121 0 0
		end
		Lifespan 61
	End
	Event
		EName	R6
		Type	local
		At	o6
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc06RUNE__PylonDisc
		bhvroverride
			PositionOffset		-.121 -7.923 0
		end
		Lifespan 61
	End
end
###
Condition   
	On	time
	Time	0
	chance .15
	Event
		EName	o7
		Type	local
		At	Base
		bhvr	:RuneRiseBig.bhvr
		bhvroverride
			PositionOffset		-.111 0 0
		end
		Lifespan 61
	End
	Event
		EName	R7
		Type	local
		At	o7
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc07RUNE__PylonDisc
		bhvroverride
			PositionOffset		.111 -9.052 0
		end
		Lifespan 61
	End
end
###
Condition   
	On	time
	Time	0
	chance .15
	Event
		EName	o8
		Type	local
		At	Base
		bhvr	:RuneRiseBig.bhvr
		bhvroverride
			PositionOffset		0 0 0
		end
		Lifespan 61
	End
	Event
		EName	R8
		Type	local
		At	o8
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc08RUNE__PylonDisc
		bhvroverride
			PositionOffset		0 -10.151 0
		end
		Lifespan 61
	End
end
