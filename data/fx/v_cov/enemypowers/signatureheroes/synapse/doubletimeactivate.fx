#########################################################
##	template

FxInfo


#########################################################
####################### AUDIO ###########################
#########################################################



#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	SpadLNode
		Type	Local
		At	SpadL	
	End
End
Condition
	On	Cycle
	Time	9
	Chance	.30


	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet SpadLNode
		POther  SpadLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
	
	End

End


Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	SpadRNode
		Type	Local
		At	SpadR	
	End
End
Condition
	On	Cycle
	Time	8
	Chance	.30


	Event	
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet SpadRNode
		POther  SpadRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
	
	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	ChestNode
		Type	Local
		At	Chest
	
	End
End
Condition
	On	Cycle
	Time	12
	Chance	.25


	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet ChestNode
		POther  ChestNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	LarmRNode
		Type	Local
		At	LarmR
	
	End
End
Condition
	On	Cycle
	Time	6
	Chance	.25
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmRNode
		POther  LarmRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	LarmRNode2
		Type	Local
		At	LarmR
	
	End
End
Condition
	On	Cycle
	Time	8
	Chance	.25
	

	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmRnode2
		POther  LarmRNode2
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	LarmLNode
		Type	Local
		At	LarmL
	
	End
End
Condition
	On	Cycle
	Time	7
	Chance	.25


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmLNode
		POther  LarmLNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End

End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	LarmLNode2
		Type	Local
		At	LarmL
	
	End
End
Condition
	On	Cycle
	Time	9
	Chance	.25


	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LarmLNode2
		POther  LarmLNode2
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	LleglNode
		Type	Local
		At	Llegl
	
	End
End
Condition
	On	Cycle
	Time	9
	Chance	.25


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LleglNode
		POther  LleglNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	LleglNode2
		Type	Local
		At	Llegl
	
	End
End
Condition
	On	Cycle
	Time	8
	Chance	.25


	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LleglNode2
		POther  LleglNode2
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	LlegRNode
		Type	Local
		At	LlegR
	
	End
End
Condition
	On	Cycle
	Time	7
	Chance	.25

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LlegRNode
		POther  LlegRNode
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End
End

Condition	## Auto-extracted from the next On Cycle condition
	Event	
		ENAME	LlegRNode2
		Type	Local
		At	LlegR
	
	End
End
Condition
	On	Cycle
	Time	6
	Chance	.25


	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:BodyGlowsLarge01.part
		part	:BodyGlowsLarge02.part
		part	:BodyArcsSmall01.part
		part	:BodyArcsSmall02.part
		Pmagnet LlegRNode2
		POther  LlegRNode2
		Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 10
		
	End


End


End		
