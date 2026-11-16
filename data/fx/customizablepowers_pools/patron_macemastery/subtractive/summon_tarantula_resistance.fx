#########################################################

FxInfo

#########################################################



Condition
	On	DEATH

	Event
		EName 	Prime
		Type	Local
		At	Origin

		Bhvr	V_COV\EnemyPowers\Guards\B_FadeOut_Entity.bhvr
		Flags	AdoptParentEntity
		Lifespan 20
	End

	Event
		EName	ChildTele
		Type	Start
		At	Root

		ChildFX	:F_TeleportationEnd_Recluse.fx
		Lifespan 110
	End

	Event
		EName	ChildPackTele
		Type	Start
		At	Root

		ChildFX	:F_TeleportationEnd_ReclusePack.fx
		Lifespan 110
	End
End

#########################################################

End