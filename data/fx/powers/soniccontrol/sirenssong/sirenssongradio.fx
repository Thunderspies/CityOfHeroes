#########################################################
##	TemperatureControl
########################################################

FxInfo

Lifespan	 80

##########################


Condition
	On 	Time
	Time 	9

	Event
		Type	local 
		At	head
		Sound SirenSong 80 80 .99
				
	End
End


Condition
	On 	Time
	Time 	14

	Event
		EName 	MussleFlashRigging
		Type	local 
		At	head
		BHVR	:GenericParticleFade.bhvr
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

End

#######################################################################################################

Condition
	On 	cycle
	Time 	8

	Event
		EName	core1
		Type	local
		At	root

		BhvrOverride
			PositionOffset	0 .8 7
		End

		Geom	SonicGaleRing1
		bhvr	:OrientScale.bhvr
		LifeSpan	8
	End

End

Condition
	On 	cycle
	Time 	8

	Event
		EName	core2
		Type	Local
		At	root

		BhvrOverride
			PositionOffset	0 .8 7
		End

		Geom	SonicGaleRing2
		bhvr	:OrientScaleOpposite.bhvr
		LifeSpan	8
	End

End

##################################################################################################

Condition
	On 	cycle
	Time 	8

	Event
		EName	core1dark
		Type	local
		At	root

		BhvrOverride
			PositionOffset	0 .8 7
		End

		Geom	SonicGaleRing1dark
		bhvr	:OrientScaledark.bhvr
		LifeSpan	8
	End

End

Condition
	On 	cycle
	Time 	8

	Event
		EName	core2dark
		Type	Local
		At	root

		BhvrOverride
			PositionOffset	0 .8 7
		End

		Geom	SonicGaleRing2dark
		bhvr	:OrientScaleOppositedark.bhvr
		LifeSpan	8
	End

End


Condition
	On 	Time
	Time 	30

		
	Event
		EName 	core1
		type	Destroy
	
	End
	
	Event
		EName 	core2
		type	Destroy
	
	End
		
	Event
		EName 	core1Dark
		type	Destroy
	
	End
	
	Event
		EName 	core2Dark
		type	Destroy
	
	End
End

End