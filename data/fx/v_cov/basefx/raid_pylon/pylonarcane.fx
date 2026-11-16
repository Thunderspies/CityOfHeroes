#########################################################
##	forceBubble
########################################################
FxInfo

########## needsound:

Flags    InheritAlpha

#################################################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	origin
		Sound StonePylon_loop 100 100 .52

	End
End


Condition
	On	Time
	Time	0

	Event
		Type	local
		At	origin
		Sound PylonMagic_loop 100 100 .30

	End
End

Condition
	On	Time
	Time	0

	Event
		EName	wanderscale
		Type	local
		At	origin
		bhvroverride
			Scale		.1 1 0.1
		end
	End
	Event
		EName	wanderspin
		Type	local
		At	wanderscale
		bhvroverride
			spinjitter		.03 .03 .03
		end 
	End
	Event
		EName	wanderoff
		Type	local
		At	wanderspin
		bhvroverride
			PositionOffset		0 .2 0
		end
	End

end


Condition
	On	Time
	Time	0

	Event
		EName	Base
		Type	positonly
		At	wanderoff
		Geom	_Raid_Pylonshaft
		bhvroverride
			PositionOffset		0 0 0
		end
	End
	Event
		EName	BaseL
		Type	positonly
		At	wanderoff
		bhvroverride
			Spin			0.0 0.01 0.0
			PositionOffset		0 0 0
		end
	End
	Event
		EName	BaseR
		Type	positonly
		At	wanderoff
		bhvroverride
			Spin			0.0 -0.01 0.0
			PositionOffset		0 0 0
		end
	End
end

######### Disks

Condition   
	On	cycle
	Time	78
	Event
		EName	RunesL
		Type	local
		At	BaseL
		childfx :PylonRunesA.fx
		lifespan 200
	End
	Event
		EName	RunesR
		Type	local
		At	BaseR
		childfx :PylonRunesB.fx
		lifespan 200
	End
end

Condition   
	On	Time
	Time	0
	Event
		EName	d1
		Type	local
		At	BaseL
		Geom	_Raid_PylonDisc01__PylonDisc01
		#bhvr	:Fadey.bhvr
	End
	Event
		EName	d2
		Type	local
		At	BaseR
		Geom	_Raid_PylonDisc02__PylonDisc01
		#bhvr	:Fadey.bhvr
	End
	Event
		EName	d3
		Type	local
		At	BaseL
		Geom	_Raid_PylonDisc03__PylonDisc01
		#bhvr	:Fadey.bhvr
	End
	Event
		EName	d4
		Type	local
		At	BaseR
		Geom	_Raid_PylonDisc04__PylonDisc01
		#bhvr	:Fadey.bhvr
	End
	Event
		EName	d5
		Type	local
		At	BaseL
		Geom	_Raid_PylonDisc05__PylonDisc01
		#bhvr	:Fadey.bhvr
	End
	Event
		EName	d6
		Type	local
		At	BaseR
		Geom	_Raid_PylonDisc06__PylonDisc01
		#bhvr	:Fadey.bhvr
	End
	Event
		EName	d7
		Type	local
		At	BaseL
		Geom	_Raid_PylonDisc07__PylonDisc01
		#bhvr	:Fadey.bhvr
	End
	Event
		EName	d8
		Type	local
		At	BaseR
		Geom	_Raid_PylonDisc08__PylonDisc01
		#bhvr	:Fadey.bhvr
	End

End



#
#Condition   
#	On	Time
#	Time	0
####
#	Event
#		EName	tr1
#		Type	local
#		At	Base
#		bhvr	:runerot.bhvr
#		Geom	_raidPylon_Runes1__runespin
#	End
#	Event
#		EName	tr2
#		Type	local
#		At	Base
#		bhvr	:runerot.bhvr
#		Geom	_raidPylon_Runes2__runespin
#	End
#	Event
#		EName	tr3
#		Type	local
#		At	Base
#		bhvr	:runerot.bhvr
#		Geom	_raidPylon_Runes3__runespin
#	End
#
#end
########## runes
#
#Condition   
#	On	Time
#	Time	0
####
#	Event
#		EName	o1
#		Type	local
#		At	Base
#		bhvroverride
#			PositionOffset		0 2.274 0
#		end
#		bhvr	:runerot.bhvr
#	End
#	Event
#		EName	R1
#		Type	local
#		At	o1
#		Geom	_Raid_PylonDisc01RUNE__PylonDisc
#		bhvroverride
#			PositionOffset		0 -2.274 0
#		end
#		#bhvr	:runerot.bhvr
#	End
####
#	Event
#		EName	o2
#		Type	local
#		At	Base
#		bhvroverride
#			PositionOffset		-.084 3.428 0
#		end
#		bhvr	:runerot.bhvr
#	End
#	Event
#		EName	R2
#		Type	local
#		At	o2
#		Geom	_Raid_PylonDisc02RUNE__PylonDisc
#		bhvroverride
#			PositionOffset		.084 -3.428 0
#		end
#		#bhvr	:runerot.bhvr
#	End
####
#	Event
#		EName	o3
#		Type	local
#		At	Base
#		bhvroverride
#			PositionOffset		.067 4.57 0
#		end
#		bhvr	:runerot.bhvr
#	End
#	Event
#		EName	R3
#		Type	local
#		At	o3
#		Geom	_Raid_PylonDisc03RUNE__PylonDisc
#		bhvroverride
#			PositionOffset		-.067 -4.57 0
#		end
#		#bhvr	:runerot.bhvr
#	End
####
#	Event
#		EName	o4
#		Type	local
#		At	Base
#		bhvroverride
#			PositionOffset		0 5.598 0
#		end
#		bhvr	:runerot.bhvr
#	End
#	Event
#		EName	R4
#		Type	local
#		At	o4
#		Geom	_Raid_PylonDisc04RUNE__PylonDisc
#		bhvroverride
#			PositionOffset		0 -5.598 0
#		end
#		#bhvr	:runerot.bhvr
#	End
####
#	Event
#		EName	o5
#		Type	local
#		At	Base
#		bhvroverride
#			PositionOffset		-.138 6.867 0
#		end
#		bhvr	:runerot.bhvr
#	End
#	Event
#		EName	R5
#		Type	local
#		At	o5
#		Geom	_Raid_PylonDisc05RUNE__PylonDisc
#		bhvroverride
#			PositionOffset		.138 -6.867 0
#		end
#	End
####
#	Event
#		EName	o6
#		Type	local
#		At	Base
#		bhvroverride
#			PositionOffset		.121 7.923 0
#		end
#		bhvr	:runerot.bhvr
#	End
#	Event
#		EName	R6
#		Type	local
#		At	o6
#		Geom	_Raid_PylonDisc06RUNE__PylonDisc
#		bhvroverride
#			PositionOffset		-.121 -7.923 0
#		end
#		#bhvr	:runerot.bhvr
#	End
####
#	Event
#		EName	o7
#		Type	local
#		At	Base
#		bhvroverride
#			PositionOffset		-.111 9.052 0
#		end
#		bhvr	:runerot.bhvr
#	End
#	Event
#		EName	R7
#		Type	local
#		At	o7
#		Geom	_Raid_PylonDisc07RUNE__PylonDisc
#		bhvroverride
#			PositionOffset		.111 -9.052 0
#		end
#	End
####
#	Event
#		EName	o8
#		Type	local
#		At	Base
#		bhvroverride
#			PositionOffset		0 10.151 0
#		end
#		bhvr	:runerot.bhvr
#	End
#	Event
#		EName	R8
#		Type	local
#		At	o8
#		Geom	_Raid_PylonDisc08RUNE__PylonDisc
#		bhvroverride
#			PositionOffset		0 -10.151 0
#		end
#	End
#end
