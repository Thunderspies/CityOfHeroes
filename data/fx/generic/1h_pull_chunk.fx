#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	origin
End

Input  
	Inpname	target
End

Input  
	Inpname	chest
End

Input  
	Inpname	WepL
End

Input  
	Inpname	WepR
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	Mystic
End

######################################################################

Lifespan 130


Condition
	On	Time
	Time	15

	Event	
		Type	Local
		At	WepR
		Sound piperampage 50 50 .55
	End
End

Condition
	On	Time
	Time	22


	Event
		EName 	pipe_ground
		Type	Local 
		At	WepR 
		Geom	GEO_WepR_prop_pipe 
		Bhvr	Behaviors/GenericParticleFade.bhvr


	End

End

Condition
	On 	Time
	Time 	143


	Event
		EName	pipe_ground
		Type	Destroy
		
	End

End




Condition
	On 	Time
	Time 	59   ##40

	Event
		EName	cracks
		Type	start
		At	mystic
		
		#Part1	POWERS\SuperStrength\SuperStrengthCrack01.part		
		Part2	POWERS\SuperStrength\superStrengthDust1.part
		part3	POWERS\SuperStrength\superStrengthDust2.part
		Part3	POWERS\EarthControl\WeaponRocks05.part
		Part4	POWERS\EarthControl\WeaponRocks05.part
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	crack
		Type	start
		At	mystic
		Part1	POWERS\SuperStrength\SuperStrengthCrack01.part		
		
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	crack
		Type	start
		At	mystic
		Part1	POWERS\SuperStrength\SuperStrengthCrack01.part		
		
	End

End






Condition
	On 	Time
	Time 	60

	Event
		EName	cracks
		Type	Destroy
		
	End
	

End

Condition
	On 	Time
	Time 	162

	Event
		EName	pipe_Impact
		Type	Local
		At	WepR		
		Part2	POWERS\SuperStrength\superStrengthDust1.part
		part3	POWERS\SuperStrength\superStrengthDust2.part
	End

End


Condition
	On 	Time
	Time	165
	
	Event
		EName 	pipe_Impact
		Type	Destroy
		

	End
	

End



End
##################################

