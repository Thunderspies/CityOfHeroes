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

Lifespan 200

Condition
	On 	Time
	Time 	30

	Event
		EName	concrete
		Type	local
		At	wepR
		geom	toss
		bhvr	behaviors/projectileSuperStrength.bhvr
				
	End
	
	Event
		EName	Quake
		Type	local
		At	chest
		
		bhvr	behaviors/SlightQuake.bhvr
				
	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName	cracks
		Type	start
		At	Mystic
		
		#Part1	POWERS\SuperStrength\SuperStrengthCrack01.part		
		Part2	POWERS\SuperStrength\superStrengthDust1.part
		part3	POWERS\SuperStrength\superStrengthDust2.part
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName	crack
		Type	start
		At	mystic
		Part1	POWERS\SuperStrength\SuperStrengthCrack01.part		
		
	End

End

Condition
	On 	Time
	Time 	28

	Event
		EName	crack
		Type	start
		At	mystic
		Part1	POWERS\SuperStrength\SuperStrengthCrack01.part		
		
	End

End



Condition
	On 	Time
	Time 	92


	Event
		EName	concrete
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	93

	Event

		EName 	Prime
		Type	Local 
		At	Mystic
		geom	toss
		Sound Rockthrow 50 50 .5
	
		
	End

End

Condition
	On 	Time
	Time 	97

	Event
		EName	rock_Impact
		Type	Local
		At	Mystic		
		Part2	POWERS\SuperStrength\superStrengthDust1.part
		part3	POWERS\SuperStrength\superStrengthDust2.part
	End

End


Condition
	On 	Time
	Time	135
	
	Event
		EName 	rock_Impact
		Type	Destroy
		

	End
	

End



Condition
	On 	Time
	Time	140
	
	Event
		EName 	Prime
		Type	Destroy
		

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



End
##################################

