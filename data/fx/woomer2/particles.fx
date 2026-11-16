#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Origin
End

LifeSpan 5000

Condition
	Event
		Type	Start
		At	Origin
		Geom	FX_IceWeapon__Ice
		Part	:particle_cube.part
		Part	:particle_cube.part
		Part	:particle_cube.part
		Part	:particle_cube.part
	End
End

End	


			