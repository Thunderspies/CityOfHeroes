#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	FootL
End

Input  
	InpName	FootR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	Head
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
	InpName	Hips
End

Input  
	InpName	chest
End

Input  
	InpName	Root
End

Input  
	InpName	Col_L
End

Input  
	InpName	Col_R
End

##################################


Condition
	On	Time
	Time	0
	
	
	Event
		EName 	hookup
		Type	Local
		At	Head
		Geom	Minion_Eyey	
		
	End

	Event
		EName 	hookupwings
		Type	Local
		At	Chest
		Geom	BehemothWings	
		
	End

End

Condition
	On 	cycle
	Time	110
	Chance	1

	Event
		EName 	Prime
		Type	Local 
		At	hookup
		altpiv	4
		part1	EnemyFx/Circle/EntityEffects/CircleOfThorns/SnortSmokeRight.part
		
		lifespan	25
	End

	Event
		EName 	headhook
		Type	Local
		At	hookup
		altpiv	5
		part1	EnemyFx/Circle/EntityEffects/CircleOfThorns/SnortSmokeLeft.part
		
		lifespan	25
	End

End

Condition
	On	Time
	Time	0	
	
	Event
		EName 	wingR
		Type	Local 
		At	hookupwings
		altpiv	1
		part	:EnvoyWing.part
		part	:EnvoyWingEmber.part
		
	End

	Event
		EName 	wingL
		Type	Local 
		At	hookupwings
		altpiv	2
		part	:EnvoyWing1.part
		part	:EnvoyWingEmber1.part
		
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:EnvoySmoke.part
		part2	:EnvoySmoke2.part
		
		Sound miasma_loop 60 60 .79
		
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:EnvoySmokea.part
		part2	:EnvoySmoke2a.part
		
		PMagnet LLEGL
		Sound obsidian_loop 60 60 .49

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:EnvoySmoke.part
		part2	:EnvoySmoke2.part
		
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:EnvoySmoke.part
		part2	:EnvoySmoke2.part
		
		Sound rage 100 100 1.0
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:EnvoySmokea.part
		part2	:EnvoySmoke2a.part
		
		PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:EnvoySmoke.part
		part2	:EnvoySmoke2.part
		
		PMagnet FootR
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	chest
		part1	:EnvoySmokea.part
		part2	:EnvoySmoke2a.part
		
	End

	Event
		EName	headtocolL
		Type	Local
		At	Head
		part1	:EnvoySmoke.part
		part2	:EnvoySmoke2.part
		PMagnet Col_L
	End

	Event
		EName	headtocolR
		Type	Local
		At	Head
		part1	:EnvoySmoke.part
		part2	:EnvoySmoke2.part
		PMagnet Col_R
	End

	Event
		EName	headtocolL
		Type	Local
		At	Col_L
		part1	:EnvoySmokea.part
		part2	:EnvoySmoke2a.part
		
	End

	Event
		EName	headtocolR
		Type	Local
		At	Col_R
		part1	:EnvoySmokea.part
		part2	:EnvoySmoke2a.part
		
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:EnvoySmokea.part
		part2	:EnvoySmoke2a.part
		
		PMagnet LArmL

	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:EnvoySmokea.part
		part2	:EnvoySmoke2a.part
	
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:EnvoySmoke.part
		part2	:EnvoySmoke2.part
		
		PMagnet WepR
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:EnvoySmoke.part
		part2	:EnvoySmoke2.part
		
		PMagnet WepL
	End

	Event
		EName	14
		Type	Local
		At	WepR
		part1	:EnvoySmokeSmall.part
		part2	:EnvoySmoke2Small.part
		
		PMagnet LArmR
	End

	Event
		EName	11
		Type	Local
		At	WepL
		part1	:EnvoySmokeSmall.part
		part2	:EnvoySmoke2Small.part
		
		PMagnet LArmL
	End

	
End    


End	

