#########################################################
## Lord Recluse - Arachnos PTT Buff
#########################################################

FxInfo

#Lifespan	210

#########################################################
## Electic Aura
#########################################################

Condition
	On	Cycle
	Time	22
	#Chance	0.5

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet SpadL
		POther  SpadL
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	20
	#Chance	0.5

	Event	
		ENAME	ChestGlow2
		Type	Local
		At	Hips

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet SpadR
		POther  SpadR
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	31
	#Chance	0.5

	Event	
		ENAME	RightUarmGlow1
		Type	Local
		At	UarmR

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet LarmR
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	36
	#Chance	0.5
	
	Event	
		ENAME	RightLarmGlow2
		Type	Local
		At	WepR

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet LarmR
		POther  LarmR
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
		
	End

End

Condition
	On	Cycle
	Time	40
	#Chance	0.5

	Event	
		ENAME	LeftUarmGlow1
		Type	Local
		At	UarmL

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet LarmL
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
		
	End

End

Condition
	On	Cycle
	Time	29
	#Chance	0.5

	Event	
		ENAME	LeftLarmGlow2
		Type	Local
		At	WepL

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet LarmL
		POther  LarmL
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
		
	End
End

Condition
	On	Cycle
	Time	37
	#Chance	0.5

	Event	
		ENAME	LeftULegGlow1
		Type	Local
		At	UlegL

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet Llegl
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
		
	End
End

Condition
	On	Cycle
	Time	28
	#Chance	0.5

	Event	
		ENAME	LeftLLegGlow2
		Type	Local
		At	FootL

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet Llegl
		POther  Llegl
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	32
	#Chance	0.5

	Event	
		ENAME	RightULegGlow1
		Type	Local
		At	UlegR

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet LlegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr
	
		Lifespan 10
	End
End

Condition
	On	Cycle
	Time	30
	#Chance	0.5

	Event	
		ENAME	RightLLegGlow2
		Type	Local
		At	FootR

		part	:Aura_Glow1.part
		part	:Aura_Glow2.part
		part	:Aura_Lightning1.part

		Pmagnet LlegR
		POther  LlegR
		Bhvr	Behaviors/TelportationFade.bhvr

		Lifespan 10
	End
End

End