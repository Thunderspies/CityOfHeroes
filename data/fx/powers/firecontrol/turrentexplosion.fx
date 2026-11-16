#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90

Input  
	Inpname	Origin
End

Input  
	Inpname	root
End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName 	lazer
		Type	Local 
		At	root
				
		Geom	roottokneesadjustment
		lifespan	90

	End

	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/vanish.bhvr
		Flags	AdoptParentEntity
		lifespan	90
	End

	Event
		
		Type	Local 
		At	lazer
		Altpiv	1
		
		Part2	powers/fireControl/Inferno2.part
		Sound inferno4 80 80 .9
		lifespan	90

	End

	Event
		EName 	Primex
		Type	Local 
		At	lazer
		Altpiv	1
		Part1	Powers/FireControl/TurrentBits.part	
		lifespan	90
		
	End

	Event
		EName 	Primex
		Type	Local 
		At	lazer
		Altpiv	1
		Bhvr	Behaviors/CameraShake03.bhvr

		Part	WeaponFx/SmokeExplosion.part
		Part	WeaponFx/fireExplosion2.part

		lifespan	90		
	
	End

End

Condition
	On 	Time
	Time	22

	Event
		EName 	lazer
		Type	Destroy
	End

	Event
		EName 	Primex
		Type	Destroy
	End

End

End
