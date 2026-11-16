#############################################################
## Judgment_Ion.fx
#############################################################

FxInfo
LifeSpan	360

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			StartJitter	13 0 13
		End
		ChildFX	:LightningStorm_Tell.fx
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			StartJitter	13 0 13
		End
		ChildFX	:CHILD_LightningStorm.fx
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Root
		BhvrOverride
			StartJitter	13 0 13
		End
		ChildFX	:CHILD_LightningStorm.fx
		Lifespan 10
	End


End


#############################################################

End