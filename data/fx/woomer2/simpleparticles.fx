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
		#Part	:simpleparticles.part
		#Part	ENEMYFX/greenblobby/greenblobby4.part
		Part	POWERS/SUPERSPEED/SPEEDEMBERLOCAL.PART
	End
End

End	


			