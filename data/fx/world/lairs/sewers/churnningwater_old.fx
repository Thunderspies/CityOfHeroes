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
		Geom	ChurnCenter
		bhvr	behaviors/Churn_spin.bhvr
		part1	World/Lairs/Sewers/Mist_OLD.part
		part2	World/Lairs/Sewers/MistDiffuse_OLD.part

		part1	World/Lairs/Sewers/MistBits_OLD.part
		part2	World/Lairs/Sewers/MistDiffuseBits_OLD.part
	End
	
	Event
		EName 	bubbles
		Type	Local 
		At	Origin
	
		part3	World/Lairs/Sewers/MistFlat_OLD.part
	End

	Event
		EName 	OrbSpot1
		Type	Local 
		At	bubbles
		  AltPiv 1
		Part1	World/Lairs/Sewers/MistBall_OLD.part
		Pmagnet	Origin
	End

	Event
		EName 	OrbSpot2
		Type	Local 
		At	bubbles
		  AltPiv 2
		Part1	World/Lairs/Sewers/MistBall_OLD.part
		Pmagnet	Origin
	End

	Event
		EName 	OrbSpot3
		Type	Local 
		At	bubbles
		  AltPiv 3
		Part1	World/Lairs/Sewers/MistBall_OLD.part
		Pmagnet	Origin
	End

	Event
		EName 	OrbSpot4
		Type	Local 
		At	bubbles
		  AltPiv 4
		Part1	World/Lairs/Sewers/MistBall_OLD.part
		Pmagnet	Origin
	End


	Event
		EName 	OrbSpot5
		Type	Local 
		At	bubbles
		  AltPiv 5
		Part1	World/Lairs/Sewers/MistBall_OLD.part
		Pmagnet	Origin
	End



	Event
		EName 	OrbSpot6
		Type	Local 
		At	bubbles
		  AltPiv 6
		Part1	World/Lairs/Sewers/MistBall_OLD.part
		Pmagnet	Origin
	End


	Event
		EName 	OrbSpot7
		Type	Local 
		At	bubbles
		  AltPiv 7
		Part1	World/Lairs/Sewers/MistBall_OLD.part
		Pmagnet	Origin
	End


	Event
		EName 	OrbSpot8
		Type	Local 
		At	bubbles
		  AltPiv 8
		Part1	World/Lairs/Sewers/MistBall_OLD.part
		Pmagnet	Origin
	End


End

End			