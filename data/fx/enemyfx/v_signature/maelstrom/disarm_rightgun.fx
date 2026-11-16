#############################################################
## Mercenary Mastermind Assault Rifle - Slug
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	WepR
		part	:SparkGlow_Equipment.part
		part	:Sparks_Flying.part
		Part	:Shockwave.part
		part	:SparkGlow_Equipment_Highlight.part
		part	:SparkBurst_Equipment.part
		Lifespan 3
		Lifespan 2
	End
End

#############################################################

End