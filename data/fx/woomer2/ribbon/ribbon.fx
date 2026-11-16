#########################################################
##	Fire_Ball
##
FxInfo

Condition
	On TriggerBits
	TriggerBits AIR
	DoMany 1

	Event
		EName   Ribbons
		While	AIR
		Type	Local
		Flags	OneAtATime
		At	Root
		Geom	FX_IceWeapon__Ice
		Part1	:ribbon.part
	End

End

End	


			