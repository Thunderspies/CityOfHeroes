#########################################################
##	Inferno FX
##
FxInfo
	
LifeSpan 1000

Input  
	InpName	Root
End

Input  
	Inpname	HIPS
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Monkey
		Type	Start
		At	HIPS
		Bhvr	behaviors/woomer/woomer.bhvr
		Geom	FX_FireWeapon__Ice
		#Part1	powers/fireControl/InfernoExplosion.part
		#Anim	hoverbot
	End

End



End			