#############################################################
## Disruption Field
#############################################################

FxInfo

#############################################################

#Condition
#	On	Time
#	Time 	7
#
#	Event
#		Type 	Local
#		At	Origin
#		Sound sonichands 80 80 .8
#	End
#End
#
#Condition
#	On	Time
#	Time 	17
#
#	Event
#		Type 	Local
#		At	Origin
#	#	Sound sonic2_loop 100.0 100.0 .77
#		bhvr	Behaviors\soundFade3.bhvr
#		Flags	PowerLoopingSound
#		Lifespan 30
#	End
#End
#
#Condition
#	On 	Time
#	Time 	30
#
#	Event
#		Type	Local
#		At	Chest
#	#	Sound sonic3 70 70 .75
#	End
#End

## HANDS ###########################################################

Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Clarion_Continuing_Loop 100 100 .2
		bhvr	behaviors/Sound/SoundIn05Out10.bhvr
		lifespan 90
	End

	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_Rings_Continuing.part
	End


	Event
		Type	Local
		At	UArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_WhiteNoise_Limbs_Continuing.part
		Part	:Clarion_Rings_Limbs_Continuing.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_WhiteNoise_Limbs_Continuing.part
		Part	:Clarion_Rings_Limbs_Continuing.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_WhiteNoise_Limbs_Small_Continuing.part
		POther	WepL
	End

	Event
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_WhiteNoise_Limbs_Small_Continuing.part
		POther	WepR
	End

	Event
		Type	Local
		At	ULegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_WhiteNoise_Limbs_Continuing.part
		Part	:Clarion_Rings_Limbs_Continuing.part
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_WhiteNoise_Limbs_Continuing.part
		Part	:Clarion_Rings_Limbs_Continuing.part
		POther	LLegR
	End

	Event
		Type	Local
		At	LLegL
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_WhiteNoise_Limbs_Continuing.part
		Part	:Clarion_Rings_Limbs_Continuing.part
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Clarion_WhiteNoise_Limbs_Continuing.part
		Part	:Clarion_Rings_Limbs_Continuing.part
		POther	FootR
	End
End

#############################################################

End