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
		Type	Local
		At	Hips
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 1.0
		End
		Part	:Stimulant_Rings.part
		Part	:Stimulant_Rings_Static.part
		Part	:Stimulant_Glow_Back.part
	End

End

#########################################################################################


End


