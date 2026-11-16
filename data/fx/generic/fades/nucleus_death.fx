#########################################################
##	template

FxInfo

Input
	Inpname	Origin
End

Input
	Inpname	Target
End

Input
	Inpname	Hips
End

Input
	Inpname	Head
End

Input
	Inpname	Eyes
End

Input
	Inpname	Root
End

Input
	Inpname	Chest
End

Input
	Inpname	Waist
End

Flags    InheritAlpha

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Waist
		Part	:Death_Flash_Huge_01.part
		Lifespan	50
	End

	Event
		EName 	Hydra_Head_Explosion
		Type	Local
		At	Root
		Part	:Death_Flash_Huge_02.part
		Lifespan	50
	End
End

