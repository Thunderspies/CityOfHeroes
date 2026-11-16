#############################################################
## MonitorSphere_Combat.fx
#############################################################

FxInfo
Lifespan 60

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	MonitorSphereAnchor
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			Scale		1.1 1.1 1.1
			StartColor	255 255 0
		End
		Geom	GEO_MonitorSphereGlowBand_Additive
		Sound PPD_MonitorSphere_TacticalAppraisal_01 100 100 .8
	End

	Event
		Type	Local
		At	MonitorSphereAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Tactics_Rings.part
		Part	:Tactics_Glow.part
		Part	:Tactics_Glow_Back.part
	End

End

#########################################################################################


End


