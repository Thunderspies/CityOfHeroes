#############################################################
## Inspiration.fx
#############################################################

FxInfo

Lifespan 160

#############################################################

Condition
	On 	Time
	Time 	0
	Event
		Type	Local
		At	Root
		Sound 	SP_Inspiration_01 100 100 .25
	End

	Event
		Type	Start
		At	Hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset  0 5 0
		End
		Part	:Lightbulb.part
		Part	:Lightbulb_Glow.part
		Lifespan 60
	End
End
#############################################################
End