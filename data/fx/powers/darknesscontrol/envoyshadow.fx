#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	root
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

#LifeSpan	250

############################################################

Condition
	On 	Time
	Time 	5

	Event	

		ENAME	HandR
		Type	local
		At	WepR

		Part1	:EnvoyHand.part
		Part3	:EnvoyHandsEmber.part
		Sound miasmaburst 100 100 .6

	End

	Event	

		ENAME	HandL
		Type	local
		At	WepL
		
		Part1	:EnvoyHand.part
		Part3	:EnvoyHandsEmber.part
	End
	
End

Condition
	On 	Time
	Time 	35

	Event
		EName 	OrbMist
		Type	posit 
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFast.bhvr
		
				
	End

	Event
		EName 	OrbStreak
		Type	posit 
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFast.bhvr
				
	End

	Event
		EName 	Orb
		Type	Local 
		At	OrbStreak
		altpiv  1
		part1	:EnvoyMistTrail.part
		part2	:EnvoyMistTrail2.part
		Sound miasma_loop 100 100 .5
			
		
	End

	Event
		EName	darkskirt
		Type	posit #Start
		At	ROOT
		
		Part1	:EnvoyPuddle.part
		Part2	:EnvoyPuddle2.part
	End

	Event	

		ENAME	BaseTrail
		Type	posit
		At	root

		Part1	:EnvoyTrail.part
		Part2	:EnvoyTrail2.part
		

	End

	Event
		EName	darkskirt
		Type	posit #Start
		At	ROOT
		
		Part1	:EnvoyPuddleFlat.part
		Part2	:EnvoyPuddle2Flat.part
	End

End

Condition
	On 	Time
	Time 	185

Event
		EName 	OrbStreak2
		Type	posit 
		At	root
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFast.bhvr
				
	End

	Event
		EName 	Orb2
		Type	Local 
		At	OrbStreak2
		altpiv  1
		part1	:EnvoyMistTrail.part
		part2	:EnvoyMistTrail2.part
			
		
	End


End
	

End			