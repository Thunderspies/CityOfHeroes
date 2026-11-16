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

	Event
		EName 	rigging
		Type	local 
		At	head
		Geom	PsionicRigging

		BhvrOverride
			PYRRotate	0 0 0
		End
			
				
	End

	Event
		EName 	FrontSpeakerLeft
		Type	local 
		At	head
		Geom	PsionicRigging	

		BhvrOverride
			PYRRotate	-30 -50 0
			PositionOffset	-1.2 .2 0
		End
				
	End

	Event
		EName 	FrontSpeakerRight
		Type	local 
		At	head
		Geom	PsionicRigging	

		BhvrOverride
			PYRRotate	-30 50 0
			PositionOffset	0.6 0 0
		End
				
				
	End

	Event
		EName 	RearSpeakerLeft
		Type	local 
		At	head
		Geom	PsionicRigging	
			
		BhvrOverride
			PYRRotate	30 130 -110
			PositionOffset	.6 1.3 -.6
		End	
	End

	Event
		EName 	RearSpeakerRight
		Type	local 
		At	head
		Geom	PsionicRigging	
			
		BhvrOverride
			PYRRotate	-10 220 0
			PositionOffset	-.8 1.3 -.6
		End	

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
		Type	start 
		At	rigging
		altpiv	3
		Part	:MussleFlashRingCoreSmall.part
		Part	:MussleFlashRingSmall.part
		LifeSpan	20
				
	End

	Event
		EName	core3
		Type	start
		At	FrontSpeakerLeft
		  Altpiv   3
		part	:FreakShow_MassiveMegaSonicRingCore.part
		part	:FreakShow_MassiveMegaSonicRing.part	
		part	:FreakShow_MassiveMegaSonicRingCore2.part
		part	:FreakShow_MassiveMegaSonicRing2.part		
		
		LifeSpan	50
	End

	Event
		EName	core3
		Type	start
		At	FrontSpeakerLeft
		  Altpiv   3
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part		
		
		LifeSpan	20
	End
	
#######################################################################################

	Event
		EName	core3
		Type	start
		At	FrontSpeakerRight
		  Altpiv   3
		part	:FreakShow_MassiveMegaSonicRingCore.part
		part	:FreakShow_MassiveMegaSonicRing.part	
		part	:FreakShow_MassiveMegaSonicRingCore2.part
		part	:FreakShow_MassiveMegaSonicRing2.part		
		
		LifeSpan	50
	End

	Event
		EName	core3
		Type	start
		At	FrontSpeakerRight
		  Altpiv   3
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part		
		
		LifeSpan	20
	End

##########################################################################################

	Event
		EName	core3
		Type	start
		At	RearSpeakerLeft
		  Altpiv   3
		part	:FreakShow_MassiveMegaSonicRingCore.part
		part	:FreakShow_MassiveMegaSonicRing.part	
		part	:FreakShow_MassiveMegaSonicRingCore2.part
		part	:FreakShow_MassiveMegaSonicRing2.part		
		
		LifeSpan	50
	End

	Event
		EName	core3
		Type	start
		At	RearSpeakerLeft
		  Altpiv   3
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part		
		
		LifeSpan	20
	End

###########################################################################################3

	Event
		EName	core3
		Type	start
		At	RearSpeakerRight
		  Altpiv   3
		part	:FreakShow_MassiveMegaSonicRingCore.part
		part	:FreakShow_MassiveMegaSonicRing.part	
		part	:FreakShow_MassiveMegaSonicRingCore2.part
		part	:FreakShow_MassiveMegaSonicRing2.part		
		
		LifeSpan	50
	End

	Event
		EName	core3
		Type	start
		At	RearSpeakerRight
		  Altpiv   3
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part		
		
		LifeSpan	20
	End

##############################################################################

	Event
		Ename	PushForce
		Type	Local
		At	rigging
		  Altpiv   3
		
		BhvrOverride
			PositionOffset		0 0 16
			pyrRotate		15 0 0
			PhysForceType		Forward
			PhysForceRadius		32
			PhysForcePower		300
			PhysForcePowerJitter	50
		End

		LifeSpan	50
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


			