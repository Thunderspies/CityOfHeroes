#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90

Input  
	Inpname	Origin
End

Input  
	Inpname	chest
End


Condition
	#On 	triggerbits
	#Triggerbits	death

	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/vanish.bhvr
		Flags	AdoptParentEntity
		lifespan	90
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		400
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End
	Event
		
		Type	Local 
		At	chest
		
		Part2	powers/fireControl/Inferno2.part
		Sound inferno4 80 80 .9
		lifespan	90

	End

	Event
		EName 	Primex
		Type	Local 
		At	Chest
		LifeSpan	10
		Part1	:TurrentBits.part	
		lifespan	90
		
	End

	Event
		EName 	Primex
		Type	Local 
		At	chest
		Bhvr	Behaviors/CameraShake03.bhvr
		
		Part	WeaponFx/SmokeExplosion.part
		#Part2	powers/fireControl/InfernoExplosion.part
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
