#############################################################
## SonicPylons_Hit.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type Local
		At Root
		Sound SonicPylon_DamageTeleport_01 300 300 .7
	End
		
	Event
		Type	Start
		CAMERASPACE
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Rings_Blast_Teleport.part
		Part	:Sonic_Blast_Teleport.part
		Lifespan 10
	End

	Event
		Type	Start
		CAMERASPACE
		Part	:Sonic_Noise_Linger.part
		Lifespan	45
	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/CameraShake02.bhvr
		Lifespan 15

	End


	Event
		Type	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Rings_Teleport.part
		POther	Root
		Lifespan 20
	End

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Sonic_Glow_Teleport.part
		POther	Root
		Lifespan 20
	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 0.25 0
		End
		Part	:Sonic_Glow_Teleport_Flat.part
		Lifespan 20
	End


	Event
		Type	Local
		At	Root
		Bhvr	behaviors\fadeOut.bhvr
		BhvrOverride
			FadeOutLength		50
		End

		Flags	AdoptParentEntity
		Lifespan 1
	End
End

#############################################################

End