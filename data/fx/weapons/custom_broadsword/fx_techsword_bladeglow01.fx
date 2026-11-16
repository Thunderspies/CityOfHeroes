#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	TalsorianBlades_Glow
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0 0 0.5
			PyrRotate	-90 0 0
		End
		Part	:TechSword_Glow01.part
	End
End

Condition
	On 	Cycle
	Time 	10
	Chance	0.25

	Event
		Type	Local
		At	Origin
		ChildFX	:FX_TechSword_Lightning01.fx
		Lifespan 25
	End
End

Condition
	On 	Cycle
	Time 	7
	Chance	0.25

	Event
		Type	Local
		At	Origin
		ChildFX	:FX_TechSword_Lightning01.fx
		Lifespan 25
	End
End

#########################################################

End