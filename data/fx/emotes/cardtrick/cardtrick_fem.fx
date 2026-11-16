#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

Lifespan 252

#############################################################

Condition

	Event
		EName	CardTrick
		Type	Local
		At	Mystic
		Anim	FX_CardTrick_Fem
		Lifespan 300
	End
End

#############################################################

Condition
	On	Time
	Time	48 #58

	Event
		EName	Poof
		Type	Start
		At	WepL
		ChildFX	Emotes\CardTrick\Child_MagicSmoke.fx
	End
End

Condition
	On	Time
	Time	83 #93

	Event
		EName	Poof
		Type	Start
		At	WepL
		ChildFX	Emotes\CardTrick\Child_MagicSmoke.fx
	End
End

#Condition
#	On	Time
#	Time	122
#
#	Event
#		EName	Poof
#		Type	Start
#		At	WepR
#		ChildFX	Emotes\CardTrick\Child_MagicSmoke.fx
#	End
#End

#############################################################

#Condition
#	On	Time
#	Time	252
#
#	Event
#		EName	TossCardsIntoAir
#		Type	Start
#		At	WepR
#		ChildFX	Emotes\CardTrick\Child_CardFountain.fx
#	End
#End

#############################################################

End