#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	100

########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	POWERS/ElectricityControl/Red_ElectricityControl01.part
		Part2	POWERS/ElectricityControl/Red_ElectricityControl02.part
		Part3	POWERS/ElectricityControl/Red_ElectricityInnerGlow.part
		Sound eleccontrol2 50 50 .4
		LifeSpan	25

	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	POWERS/ElectricityControl/Red_ElectricityControl01.part
		Part2	POWERS/ElectricityControl/Red_ElectricityControl02.part
		Part3	POWERS/ElectricityControl/Red_ElectricityInnerGlow.part
		LifeSpan	25
		
	End


	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	POWERS/ElectricityControl/Red_ElectricityOuterGlow.part
		Part2	POWERS/ElectricityControl/Red_ElectricityArch.part
		Part3	POWERS/ElectricityControl/Red_ElectricityRing.part
		Part4	POWERS/ElectricityControl/Red_ElectricityArchInvert.part
		LifeSpan	25
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	POWERS/ElectricityControl/Red_ElectricityOuterGlow.part
		Part2	POWERS/ElectricityControl/Red_ElectricityArch.part
		Part3	POWERS/ElectricityControl/Red_ElectricityRing.part
		Part4	POWERS/ElectricityControl/Red_ElectricityArchInvert.part
		LifeSpan	25
		
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
	
		At	origin
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\Redmu_LightningBlastArc.fx
		Sound ThunderCrack2 80 80 .89
		lifespan 40
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	Local
	
		At	origin
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\Redmu_LightningBlastArc.fx
		lifespan 40
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	Local
	
		At	origin
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\Redmu_LightningBlastArc.fx
		lifespan 40
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
	
		At	origin
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\Redmu_LightningBlastArc.fx
		lifespan 40
	End
End


Condition
	On 	Time
	Time 	10

	Event
		Type	Local
	
		At	origin
		Sound lightningbolthit 100 100 .7
		lifespan 40
	End
End



