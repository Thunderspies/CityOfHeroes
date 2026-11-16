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
	Time 	7	#0

	Event
		Type	Local
	
		At	origin
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\mu_LightningBlastArc.fx
		Sound ThunderCrack2 80 80 .89
		lifespan 40
	End
End

Condition
	On 	Time
	Time 	12	#2

	Event
		Type	Local
	
		At	origin
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\mu_LightningBlastArc.fx
		lifespan 40
	End
End

Condition
	On 	Time
	Time 	14	#4

	Event
		Type	Local
	
		At	origin
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\mu_LightningBlastArc.fx
		lifespan 40
	End
End

Condition
	On 	Time
	Time 	16	#6

	Event
		Type	Local
	
		At	origin
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\mu_LightningBlastArc.fx
		lifespan 40
	End
End


Condition
	On 	Time
	Time 	20	#10

	Event
		Type	Local
	
		At	origin
		Sound lightningbolthit 100 100 .7
		lifespan 40
	End
End



