#############################################################
## Overload_Attack.fx
#############################################################

FxInfo
Lifespan 200

#############################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Awakened_Overload_Attack_01 100 100 .8
	End
End


Condition
	On	Time
	Time	80

	Event
		Type	Local
		At	Head
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			FadeOutLength	1
		End
		Flags	AdoptParentEntity
		ChildFX	:CHILD_Overload_BodyGlow.fx
		Lifespan 10
	End
End

#############################################################

End