#########################################################
##	FireRing
##
FxInfo
	

Input  
	InpName	Origin
End

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	OrbMist
		Type	Local 
		At	Origin
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFast.bhvr
		part1	World/Lairs/COT/RuneMist.part
		
	End

	Event
		EName 	OrbStreak
		Type	Local 
		At	Origin
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFaster.bhvr
				
	End

	Event
		EName 	Orb
		Type	Local 
		At	OrbStreak
		altpiv  1
		Part1	World/Lairs/COT/GlowOrbStar2.part
		part2	World/Lairs/COT/GlowOrbTail.part
			
		
	End

End	


Condition
	On 	Time
	Time 	110

Event
		EName 	OrbStreak2
		Type	Local 
		At	Origin
		geom	FX_RuneStreak
		bhvr	behaviors/Rune_spinFaster.bhvr
				
	End

	Event
		EName 	Orb2
		Type	Local 
		At	OrbStreak2
		altpiv  1
		Part1	World/Lairs/COT/GlowOrbStar2.part
		part2	World/Lairs/COT/GlowOrbTail.part
			
		
	End




End

End			