#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Hips
End

Lifespan	200

########################################################################


Condition
	On 	Time
	Time 	0

#	Event
#		EName	roothookup
#		Type	local
#		At	root
#		Bhvr	behaviors/alpha.bhvr
#		Geom	RootToChestAdjustment
#	End

	Event
		EName	hookup
		Type	local
#		At	roothookup
#		altpiv  1
#		Geom	LungeNode
		At	Chest
		BhvrOverride
			PositionOffset 0 0 2
		End
	End
End

Condition
	On 	Time
	Time 	17

	Event
		EName	Prime
		Type	startPositOnly
		At	hookup
		#altpiv	2
		Geom	GEO_WepR_Shuriken
		Bhvr	ENEMYFX\Tsoo\Shuriken.bhvr
		BhvrOverride
			#Scale			2 2 2
			#Spin			0.0 -1.5 0
			InitialVelocity		0 0 0
			#InitialVelocityJitter	0.5 0.5 0
		End
		Part1	ENEMYFX\Tsoo\ShurikenStreaks01.part

		Magnet	Target
		LookAt	Target
	End

End


Condition
	On 	Time
	Time 	23

	Event
		Ename LstreakAnchor
		Type local
		At Chest
		BhvrOverride
			PositionOffset -1 0 0
		end
	End

	Event
		EName	Streak
		Type	PositOnly
		At	LstreakAnchor
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		BhvrOverride
			Scale	0.6 0.6 0.6
			PyrRotate 0 55 0
		End
		Sound clawtwirl5 100.0 100.0 .7
	End

End


Condition
	On 	Time
	Time 	28

	Event
		EName	streak
		Type	Destroy
	End
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End
End

End
##################################

