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
		EName 	bubbles
		Type	Local 
		At	Origin
		Geom	FX_RuneGlows
		bhvr	behaviors/Cauldron_spin.bhvr
		Part1	World/Lairs/COT/CauldronRing.part
		Part2	World/Lairs/COT/CauldronRingMotion.part
	End

	Event
		EName 	OrbSpot1
		Type	Local 
		At	bubbles
		  AltPiv 1
		#part1	World/Lairs/COT/CauldronGlowOrb.part
		#Part2	World/Lairs/COT/CauldronGlowOrbStar.part
		#part3	World/Lairs/COT/CauldronGlowOrb2.part
		part4	World/Lairs/COT/CauldronGlowOrbTrailWarm.part
		#Part5	World/Lairs/COT/CauldronRing.part
	End

	Event
		EName 	OrbSpot2
		Type	Local 
		At	bubbles
		  AltPiv 2
		#part1	World/Lairs/COT/CauldronGlowOrb.part
		#Part2	World/Lairs/COT/CauldronGlowOrbStar.part	
		#part3	World/Lairs/COT/CauldronGlowOrb2.part
		part4	World/Lairs/COT/CauldronGlowOrbTrailWarm.part
		#Part5	World/Lairs/COT/CauldronRing.part
	End

	Event
		EName 	OrbSpot3
		Type	Local 
		At	bubbles
		  AltPiv 3
		#part1	World/Lairs/COT/CauldronGlowOrb.part
		#Part2	World/Lairs/COT/CauldronGlowOrbStar.part
		#part3	World/Lairs/COT/CauldronGlowOrb2.part
		part4	World/Lairs/COT/CauldronGlowOrbTrailWarm.part
		#Part5	World/Lairs/COT/CauldronRing.part
	End

	Event
		EName 	OrbSpot4
		Type	Local 
		At	bubbles
		  AltPiv 4
		#part1	World/Lairs/COT/CauldronGlowOrb.part
		#Part2	World/Lairs/COT/CauldronGlowOrbStar.part
		#part3	World/Lairs/COT/CauldronGlowOrb2.part
		part4	World/Lairs/COT/CauldronGlowOrbTrailWarm.part
		#Part5	World/Lairs/COT/CauldronRing.part
	End


	Event
		EName 	OrbSpot5
		Type	Local 
		At	bubbles
		  AltPiv 5
		#part1	World/Lairs/COT/CauldronGlowOrb.part
		#Part2	World/Lairs/COT/CauldronGlowOrbStar.part
		#part3	World/Lairs/COT/CauldronGlowOrb2.part
		part4	World/Lairs/COT/CauldronGlowOrbTrailWarm.part
		#Part5	World/Lairs/COT/CauldronRing.part
	End



	Event
		EName 	OrbSpot6
		Type	Local 
		At	bubbles
		  AltPiv 6
		#part1	World/Lairs/COT/CauldronGlowOrb.part
		#Part2	World/Lairs/COT/CauldronGlowOrbStar.part
		#part3	World/Lairs/COT/CauldronGlowOrb2.part
		part4	World/Lairs/COT/CauldronGlowOrbTrailWarm.part
		#Part5	World/Lairs/COT/CauldronRing.part
	End


	Event
		EName 	OrbSpot7
		Type	Local 
		At	bubbles
		  AltPiv 7
		#part1	World/Lairs/COT/CauldronGlowOrb.part
		#Part2	World/Lairs/COT/CauldronGlowOrbStar.part
		#part3	World/Lairs/COT/CauldronGlowOrb2.part
		part4	World/Lairs/COT/CauldronGlowOrbTrailWarm.part
		#Part5	World/Lairs/COT/CauldronRing.part
	End


	Event
		EName 	OrbSpot8
		Type	Local 
		At	bubbles
		  AltPiv 8
		#part1	World/Lairs/COT/CauldronGlowOrb.part
		#Part2	World/Lairs/COT/CauldronGlowOrbStar.part
		#part3	World/Lairs/COT/CauldronGlowOrb2.part
		part4	World/Lairs/COT/CauldronGlowOrbTrailWarm.part
		#Part5	World/Lairs/COT/CauldronRing.part
	End


End

End			