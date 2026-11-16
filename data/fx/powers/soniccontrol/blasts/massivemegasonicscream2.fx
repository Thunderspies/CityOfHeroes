#############################################################
## MassiveMegaSonicScream2.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Condition
	On	Time
	Time	4

	Event
		Type	Local
		At	head
		Sound Sonic12 90 90 .9
	End
End

Condition
	On	Time
	Time	29

	Event
		Type	Local
		At	head
		Sound Sonic11 90 90 1.0
	End
End

Condition
	On 	Time
	Time 	29

	Event
		EName	core
		Type	Local
		At	head
		bhvr	:Offset2.bhvr
		Geom	LocalPivot01
		LifeSpan 10
	End

	Event
		EName	core3
		Type	start
		At	core
		Altpiv	4
		part	:MassiveMegaSonicRingCore.part
		part	:MassiveMegaSonicRing.part
		LifeSpan 10
	End

	Event
		EName	core3
		Type	start
		At	core
		Altpiv	4
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
		LifeSpan 20
	End

	Event
		EName	core3
		Type	start
		At	core
		Altpiv	4
		part	:MassiveMegaSonicRingCore2.part
		part	:MassiveMegaSonicRing2.part
		LifeSpan 10
	End

	Event
		Ename	PushForce
		Type	Local
		At	core
		Altpiv	4
		BhvrOverride
			PositionOffset		0 0 16
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		32
			PhysForcePower		300
			PhysForcePowerJitter	50
		End
		Lifespan 10
	End
End

###############################################################################################################3

#Condition
#	On	Cycle
#	Chance	0.8
#	Time	9
#
#	Event
#		EName	HeadRigging
#		Type	start
#		At	head
##		part	:MassiveMegaSonicHeadCore.part
##		part	:MassiveMegaSonicHeadCoreDiffuse.part
#		LifeSpan	1
#	End
#End
#
#Condition
#	On	Time
#	Time	3
#
#	Event
#		EName	HeadRigging
#		Type	Destroy
#	End
#End

#############################################################

End