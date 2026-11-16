#############################################################
## SonicPylons_Hit.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################
Condition
	On Time
	Time 30 
	Event
		Type Local
		At Root
		Sound SonicPylon_DamageStart_01 100 100 .9
	End
End

Condition
	On 	Cycle
	Time 	30

	Event
		Type	Start
		CAMERASPACE
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Rings_Blast.part
		Part	:Sonic_Blast.part
		Lifespan 15
	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/CameraShake03.bhvr
		Sound SonicPylon_Damage_Continuing_01 200 200 .8
		Lifespan 30

	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Start
		CAMERASPACE
		Part	:Sonic_Noise_Linger.part
	End

End

	
	
	
#############################################################

End