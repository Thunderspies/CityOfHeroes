#########################################################
##	pump_smoke
##
FxInfo

Input  
	InpName	Origin
End

LifeSpan 800


#Condition
#	On Time
#	Time 0
#	Event
#		EName 	Prime
#		Type	Local
#		At	Origin 
#		
#		Bhvr	:B_FadeOut_Entity.bhvr
#		Flags	AdoptParentEntity
#		Lifespan 285
#	End
#End

Condition
	On	Time
	Time	0

	Event
		EName	ChildTele
		Type	Local
		At	Root

		ChildFX	V_COV\EnemyPowers\Guards\F_TeleportationEnd.fx 
		Lifespan 60
	End
End

End			