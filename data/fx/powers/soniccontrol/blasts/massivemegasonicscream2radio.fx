#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	50

###########################################################################################################

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
		Geom	LocalPivot01
#		bhvr	:Offset2.bhvr	
		
		LifeSpan	10
	End

	Event
		EName 	MussleFlashRigging
		Type	local 
		At	head
#		BHVR	:GenericParticleFade.bhvr
		Geom	PsionicRigging	
				
		Lifespan	 80
	End

	Event
		Type	local 
		At	MussleFlashRigging
		altpiv	3

		BhvrOverride
			PositionOffset	.8 .2 0
		End

		Part	Powers/SonicControl/Blasts/MussleFlashRingCore.part
		Part	Powers/SonicControl/Blasts/MussleFlashRing.part
				
		Lifespan	80
	End

	Event
		Type	local 
		At	MussleFlashRigging
		altpiv	3

		BhvrOverride
			PositionOffset	-.8 .2 0
		End

		Part	Powers/SonicControl/Blasts/MussleFlashRingCore.part
		Part	Powers/SonicControl/Blasts/MussleFlashRing.part
				
		Lifespan	80
	End

	Event
		Ename	PushForce
		Type	Local
		At	core
		  Altpiv   4
		
		BhvrOverride
			PositionOffset		0 0 16
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		32
			PhysForcePower		300
			PhysForcePowerJitter	50
		End

		Lifespan	10
	End
End

###############################################################################################################3

Condition
	On	Cycle
	Chance	.8
	Time	9
	
	Event
		EName	HeadRigging
		Type	start
		At	head
#		part	:MassiveMegaSonicHeadCore.part	
#		part	:MassiveMegaSonicHeadCoreDiffuse.part	
		
		LifeSpan	1

	End

End

Condition
	On	Time
	Time	3

	Event
		EName	HeadRigging
		Type	Destroy
		
	End
End	

End


			