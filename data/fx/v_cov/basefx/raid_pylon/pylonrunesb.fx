#########################################################
##	forceBubble
########################################################
FxInfo

#lifespan 600

Flags    InheritAlpha

#################################################################################

Condition
	On	Time
	Time	0

	Event
		EName	Base
		Type	local
		At	origin
#		Geom	_Raid_Pylonshaft
#		bhvroverride
#			PositionOffset		0 0 0
#		end
#
	End
end



Condition   
	On	Time
	Time	0
###
	Event
		EName	tr1
		Type	local
		At	Base
		bhvr	:GrowSpinSmall.bhvr
		bhvroverride
			PositionOffset		0 11.27 0
		end
	End

	Event
		Type	local
		At	tr1
		Geom	_raidPylon_Runes1__runespin
		bhvr	:Fadey.bhvr
		bhvroverride
			PositionOffset		0 -11.27 0
		end

	End

end


Condition   
	On	Time
	Time	9
###
	Event
		EName	tr3
		Type	local
		At	Base
		bhvr	:GrowSpinSmall.bhvr
		bhvroverride
			PositionOffset		0 11.27 0
		end
	End

	Event
		Type	local
		At	tr3
		Geom	_raidPylon_Runes3__runespin
		bhvr	:Fadey.bhvr
		bhvroverride
			PositionOffset		0 -11.27 0 #0 -10.7 0
		end

	End

end
######### runes

Condition   
	On	Time
	Time	10

	Event
		EName	o2
		Type	local
		At	Base
		bhvroverride
			PositionOffset		-.084 3.428 0
		end
		bhvr	:GrowSpin.bhvr
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
		lifespan 61
	End 

	Event
		Type	local
		At	origin
		Geom	_Raid_PylonDisc04RUNE__PylonDisc
		bhvr	:QuickFadey.bhvr
		lifespan 60
	End
end
Condition   
	On	Time
	Time	30
	Event
		EName	o4
		Type	local
		At	Base
		bhvroverride
			PositionOffset		0 5.218 0
		end
		bhvr	:GrowSpin.bhvr
	End
	Event
		EName	R4
		Type	local
		At	o4
		bhvr	:Fadey.bhvr
		Geom	_Raid_PylonDisc04RUNE__PylonDisc
		bhvroverride
			PositionOffset		0 -5.218 0
		end
		lifespan 61
	End

	Event
		Type	local
		At	origin
		Geom	_Raid_PylonDisc06RUNE__PylonDisc
		bhvr	:QuickFadey.bhvr
		lifespan 60
	End
end
Condition   
	On	Time
	Time	50
	Event
		EName	o6
		Type	local
		At	Base
		bhvroverride
			PositionOffset		.121 7.923 0
		end
		bhvr	:GrowSpin.bhvr
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
		lifespan 61
	End

	Event
		Type	local
		At	origin
		Geom	_Raid_PylonDisc08RUNE__PylonDisc
		bhvr	:QuickFadey.bhvr
		lifespan 60
	End
end
Condition   
	On	Time
	Time	70
	Event
		EName	o8
		Type	local
		At	Base
		bhvroverride
			PositionOffset		0 10.151 0
		end
		bhvr	:GrowSpin.bhvr
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
		lifespan 61
	End

	Event
		Type	local
		At	origin
		Geom	_Raid_PylonDisc02RUNE__PylonDisc
		bhvr	:QuickFadey.bhvr
		lifespan 60
	End
end



######### Disks

#Condition   
#	On	Time
#	Time	0
#	Event
#		EName	d1
#		Type	local
#		At	Base
#		Geom	_Raid_PylonDisc01__PylonDisc01
#
#	End
#	Event
#		EName	d2
#		Type	local
#		At	Base
#		Geom	_Raid_PylonDisc02__PylonDisc01
#
#	End
#	Event
#		EName	d3
#		Type	local
#		At	Base
#		Geom	_Raid_PylonDisc03__PylonDisc01
#
#	End
#	Event
#		EName	d4
#		Type	local
#		At	Base
#		Geom	_Raid_PylonDisc04__PylonDisc01
#
#	End
#	Event
#		EName	d5
#		Type	local
#		At	Base
#		Geom	_Raid_PylonDisc05__PylonDisc01
#
#	End
#	Event
#		EName	d6
#		Type	local
#		At	Base
#		Geom	_Raid_PylonDisc06__PylonDisc01
#
#	End
#	Event
#		EName	d7
#		Type	local
#		At	Base
#		Geom	_Raid_PylonDisc07__PylonDisc01
#
#	End
#	Event
#		EName	d8
#		Type	local
#		At	Base
#		Geom	_Raid_PylonDisc08__PylonDisc01
#
#	End
#
#End

