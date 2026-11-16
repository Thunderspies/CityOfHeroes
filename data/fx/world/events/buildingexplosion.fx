#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	300

##########################################################################################################
##Final Blast
######################################################################################3

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound buildingexplo 400 400 .96
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	HandR
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		160
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	3

	End

End


Condition
	On 	Time
	Time 	6

	
	Event
		Type	Local 
		At	root
	
		Part	:InfernoExplosionGlow.part
	
		LifeSpan	10

	End

End


Condition
	On 	Time
	Time 	8

	Event
		EName 	Prime
		Type	Local 
		At	hips
				
		Bhvr	Behaviors/BuildingExplosionCameraShake.bhvr
		Part	:CoreExplosionFire.part
		Part	:CoreExplosionFire2.part

		Part	:InfernoExplosionGlow.part

		LifeSpan	10
		

	End

End

Condition
	On 	Time
	Time 	6

	Event
		EName 	Primex
		Type	Local 
		At	Root		
		Part	:InfernoExplosion.part
		LifeSpan	120
	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Primex
		Type	Local 
		At	Root
		Bhvr	:Offset.bhvr
		Part	:Sparks.part	
		Part	:BlackDeathSmoke.part
		Part	:BlackDeathSteam.part
		#LifeSpan	40
	End



	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:Inferno.bhvr
	End


	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    :Infernobubble.bhvr
		Part	:InfernoYellow2.part
		Part	:InfernoYellowFlame01.part
		Part   :Infernoflash.part
		Lifespan	800
	End
End


Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr
		Part	:fire.part
		Part	:fire.part
		Part	:fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset2.bhvr
		Part	:fire.part
		Part	:fire.part
		Part	:fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset3.bhvr
		Part	:fire.part
		Part	:fire.part
		Part	:fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset4.bhvr
		Part	:fire.part
		Part	:fire.part
		Part	:fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
	End

End

###########################################################################################


Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset.bhvr
		Part	:fire.part
		Part	:fire.part
		Part	:fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset2.bhvr
		Part	:fire.part
		Part	:fire.part
		Part	:fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset3.bhvr
		Part	:fire.part
		Part	:fire.part
		Part	:fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
	End

End

Condition
	Time 	0
	Chance	.8

	Event
		Type	start
		At	chest

		Bhvr	:StartOffset4.bhvr
		Part	:fire.part
		Part	:fire.part
		Part	:fireSparks.part
		Part	:fireSmoke.part
		Lifespan	80
	End

End

End
