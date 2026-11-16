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
	Time	4
###
	Event
		EName	tr2
		Type	local
		At	Base
		bhvr	:GrowSpinSmall.bhvr
		bhvroverride
			PositionOffset		0 11.27 0
		end
	End

	Event
		Type	local
		At	tr2
		Geom	_raidPylon_Runes2__runespin
		bhvr	:Fadey.bhvr
		bhvroverride
			PositionOffset		0 -11.27 0 #0 -11.03 0
		end

	End

end


######### runes

Condition   
	On	Time
	Time	0
###
	Event
		EName	o1
		Type	local
		At	Base
		bhvroverride
			PositionOffset		0 2.274 0
		end
		bhvr	:GrowSpin.bhvr
#		lifespan 20
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
		lifespan 61
	End

	Event
		Type	local
		At	origin
		Geom	_Raid_PylonDisc03RUNE__PylonDisc
		bhvr	:QuickFadey.bhvr
		lifespan 60
	End

end
Condition   
	On	Time
	Time	20
	Event
		EName	o3
		Type	local
		At	Base
		bhvroverride
			PositionOffset		.067 4.57 0
		end
		bhvr	:GrowSpin.bhvr
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
		lifespan 61
	End

	Event
		Type	local
		At	origin
		Geom	_Raid_PylonDisc05RUNE__PylonDisc
		bhvr	:QuickFadey.bhvr
		lifespan 60
	End
end
Condition   
	On	Time
	Time	40
	Event
		EName	o5
		Type	local
		At	Base
		bhvroverride
			PositionOffset		-.138 6.867 0
		end
		bhvr	:GrowSpin.bhvr
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
		lifespan 61
	End

	Event
		Type	local
		At	origin
		Geom	_Raid_PylonDisc07RUNE__PylonDisc
		bhvr	:QuickFadey.bhvr
		lifespan 60
	End
end
Condition   
	On	Time
	Time	60
	Event
		EName	o7
		Type	local
		At	Base
		bhvroverride
			PositionOffset		-.111 9.052 0
		end
		bhvr	:GrowSpin.bhvr
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
		lifespan 61
	End

	Event
		Type	local
		At	origin
		Geom	_Raid_PylonDisc01RUNE__PylonDisc
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

