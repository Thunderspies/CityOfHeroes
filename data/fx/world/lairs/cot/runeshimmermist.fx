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
		#part2	World/Lairs/COT/GlowOrb2.part
		part3	World/Lairs/COT/GlowOrbTail.part
			
		
	End

End	



Condition
	On 	Time
	Time 	30

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
		#part2	World/Lairs/COT/GlowOrb2.part
		part3	World/Lairs/COT/GlowOrbTail.part
			
		
	End

End


Condition
	On 	Time
	Time 	60

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
		#part2	World/Lairs/COT/GlowOrb2.part
		part3	World/Lairs/COT/GlowOrbTail.part
			
		
	End

End

Condition
	On 	Time
	Time 	90

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
		#part2	World/Lairs/COT/GlowOrb2.part
		part3	World/Lairs/COT/GlowOrbTail.part
			
		
	End

End

Condition
	On 	Time
	Time 	120

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
		#part2	World/Lairs/COT/GlowOrb2.part
		part3	World/Lairs/COT/GlowOrbTail.part
			
		
	End
End

Condition
	On 	Time
	Time 	150

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
		#part2	World/Lairs/COT/GlowOrb2.part
		part3	World/Lairs/COT/GlowOrbTail.part
			
	
	End

End


Condition
	On 	Time
	Time 	180

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
		#part2	World/Lairs/COT/GlowOrb2.part
		part3	World/Lairs/COT/GlowOrbTail.part
			
		
	End



End

End			