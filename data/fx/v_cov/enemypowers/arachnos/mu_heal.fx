#########################################################
##	Predefined arc curve
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	200

########################################################




###############Ring of Launchers#################
Condition

	On 	time
	Time 	5
	
	Event
		Type	Local
	
		At	t_root

	Sound blind 80 80 .8
	end
end


Condition

	On 	time
	Time 	15
	
	Event
		Ename	Y45
		Type	Local
	
		At	t_root
		bhvr	:mu_Heal_Y45.bhvr
		#geom    Testing_Target
	End

		
	Event
		Ename	Y90
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Y45.bhvr
		#geom    Testing_Target
	End
	
	Event
		Ename X1
		Type	Local
	
		At	Y90
		bhvr	:mu_Heal_Zpos.bhvr
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part2	:mu_Heal_OrbSpots.part
		part2	:mu_Heal_OrbSpotsFront.part
		lifespan 31
		pmagnet	T1
		#geom    Testing_Target
	End


	Event
		Ename X2
		Type	Local
	
		At	Y90
		bhvr	:mu_Heal_Zneg.bhvr
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part2	:mu_Heal_OrbSpots.part
		part2	:mu_Heal_OrbSpotsFront.part
		lifespan 31
		pmagnet	T2
		#geom    Testing_Target
#		magnet	t_head
	End

	
	Event
		Ename X3
		Type	Local
	
		At	Y90
		bhvr	:mu_Heal_Xpos.bhvr
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part2	:mu_Heal_OrbSpots.part
		part2	:mu_Heal_OrbSpotsFront.part
		lifespan 31
		pmagnet	T3
		#geom    Testing_Target
#		magnet	t_head
	End

	
	Event
		Ename X4
		Type	Local
	
		At	Y90
		bhvr	:mu_Heal_Xneg.bhvr
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part2	:mu_Heal_OrbSpots.part
		part2	:mu_Heal_OrbSpotsFront.part
		lifespan 31
		pmagnet	T4
		#geom    Testing_Target
#		magnet	t_head
	End

	########launchers @ 45##########

	Event
		Ename X5
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Zpos.bhvr
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part2	:mu_Heal_OrbSpots.part
		part2	:mu_Heal_OrbSpotsFront.part
		lifespan 31
		pmagnet	T5
		#geom    Testing_Target
	End

	
	Event
		Ename X6
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Zneg.bhvr
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part2	:mu_Heal_OrbSpots.part
		part2	:mu_Heal_OrbSpotsFront.part
		lifespan 31
		pmagnet	T6
		#geom    Testing_Target
#		magnet	t_head
	End

	
	Event
		Ename X7
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Xpos.bhvr
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part2	:mu_Heal_OrbSpots.part
		part2	:mu_Heal_OrbSpotsFront.part
		lifespan 31
		pmagnet	T7
		#geom    Testing_Target
#		magnet	t_head
	End

	
	Event
		Ename X8
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Xneg.bhvr
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part1	:mu_Heal_Steam.part
		part2	:mu_Heal_OrbSpots.part
		part2	:mu_Heal_OrbSpotsFront.part
		lifespan 31
		pmagnet	T8
		#geom    Testing_Target
#		magnet	t_head
	End
	Event
		
		Type	PositOnly
		At	Hips
		Sound elecballhit2 100 100 .5
				
	End

###########lightning going the wrong way ;)  ####
End



Condition

	On 	time
	Time 	17 



	Event
		Type	start
	
		At	X1
		childfx :mu_Heal_Branches.fx
	End


	Event
		Type	start
	
		At	X2
		childfx :mu_Heal_Branches.fx
	End


	Event
		Type	start
	
		At	X3
		childfx :mu_Heal_Branches.fx
	End


	Event
		Type	start
	
		At	X4
		childfx :mu_Heal_Branches.fx
	End

	Event
		Type	start
	
		At	X5
		childfx :mu_Heal_Branches.fx
	End


	Event
		Type	start
	
		At	X6
		childfx :mu_Heal_Branches.fx
	End


	Event
		Type	start
	
		At	X7
		childfx :mu_Heal_Branches.fx
	End


	Event
		Type	start
	
		At	X8
		childfx :mu_Heal_Branches.fx
	End




end


