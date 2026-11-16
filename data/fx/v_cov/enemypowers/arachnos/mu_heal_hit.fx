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

Condition

	On 	time
	Time 	0

	Event
		Ename	Glowy
		Type	local
	
		At	chest
		childfx	:mu_Heal_Aura.fx
#		magnet	chest
	End

end


#############Spinner Rig###################


Condition

	On 	time
	Time 	10
	
	Event
		Type	Local
	
		At	root

	Sound heal1 80 80 0.8
	end
end

Condition

	On 	time
	Time 	0

#	
#	Event
#		Ename	TargSky
#		Type	positonly
#	
#		At	chest
#		bhvr	:mu_Heal_Sky.bhvr
#		##geom    Testing_Target
##		magnet	chest
#	End
	
	
	Event
		Ename	Targ
		Type	positonly
	
		At	Chest
		bhvr	:mu_Heal_ROTOFF.bhvr
		##geom    Testing_Target
	End
	

		
	Event
		Ename	Ypos
		Type	Local
	
		At	Targ
		bhvr	:mu_Heal_YPos.bhvr
		##geom    Testing_Target
	End
		
	Event
		Ename	YNeg
		Type	Local
	
		At	Targ
		bhvr	:mu_Heal_YNeg.bhvr
		##geom    Testing_Target
	End

	Event
		Ename	RotY
		Type	Local
	
		At	Ypos
		bhvr	:mu_Heal_Rot.bhvr
		##geom    Testing_Target
	End

		
	Event
		Ename	RotYNeg
		Type	Local
	
		At	Yneg
		bhvr	:mu_Heal_RotNeg.bhvr
		##geom    Testing_Target
	End
	
############### Targets  ############


	Event
		Ename T1
		Type	Local
	
		At	RotY
		bhvr	:mu_Heal_Zpos.bhvr
		##geom    Testing_Target
	End

	
	Event
		Ename T2
		Type	Local
	
		At	RotY
		bhvr	:mu_Heal_Zneg.bhvr
		##geom    Testing_Target
#		magnet	head
	End

	
	Event
		Ename T3
		Type	Local
	
		At	RotY
		bhvr	:mu_Heal_Xpos.bhvr
		##geom    Testing_Target
#		magnet	head
	End

	
	Event
		Ename T4
		Type	Local
	
		At	RotY
		bhvr	:mu_Heal_Xneg.bhvr
		##geom    Testing_Target
#		magnet	head
	End

	##################

	Event
		Ename T5
		Type	Local
	
		At	RotYNeg
		bhvr	:mu_Heal_Zpos.bhvr
		##geom    Testing_Target
	End

	
	Event
		Ename T6
		Type	Local
	
		At	RotYNeg
		bhvr	:mu_Heal_Zneg.bhvr
		##geom    Testing_Target
#		magnet	head
	End

	
	Event
		Ename T7
		Type	Local
	
		At	RotYNeg
		bhvr	:mu_Heal_Xpos.bhvr
		##geom    Testing_Target
#		magnet	head
	End

	
	Event
		Ename T8
		Type	Local
	
		At	RotYNeg
		bhvr	:mu_Heal_Xneg.bhvr
		##geom    Testing_Target
#		magnet	head
	End
End




###############Ring of Launchers#################

Condition

	On 	time
	Time 	0
	
	Event
		Ename	Y45
		Type	Local
	
		At	root
		bhvr	:mu_Heal_Y45.bhvr
		##geom    Testing_Target
	End

		
	Event
		Ename	Y90
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Y45.bhvr
		##geom    Testing_Target
	End
	
	Event
		#Ename poo
		Type	Local
	
		At	Y90
		bhvr	:mu_Heal_Zpos.bhvr
		part1	:mu_Heal_Orb.part
		pmagnet	T1
		##geom    Testing_Target
	End

	
	Event
		#Ename poo
		Type	Local
	
		At	Y90
		bhvr	:mu_Heal_Zneg.bhvr
		part1	:mu_Heal_Orb.part
		pmagnet	T2
		##geom    Testing_Target
#		magnet	head
	End

	
	Event
		#Ename poo
		Type	Local
	
		At	Y90
		bhvr	:mu_Heal_Xpos.bhvr
		part1	:mu_Heal_Orb.part
		pmagnet	T3
		##geom    Testing_Target
#		magnet	head
	End

	
	Event
		#Ename poo
		Type	Local
	
		At	Y90
		bhvr	:mu_Heal_Xneg.bhvr
		part1	:mu_Heal_Orb.part
		pmagnet	T4
		##geom    Testing_Target
#		magnet	head
	End

	########launchers @ 45##########

	Event
		#Ename poo
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Zpos.bhvr
		part1	:mu_Heal_Orb.part
		pmagnet	T5
		##geom    Testing_Target
	End

	
	Event
		#Ename poo
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Zneg.bhvr
		part1	:mu_Heal_Orb.part
		pmagnet	T6
		##geom    Testing_Target
#		magnet	head
	End

	
	Event
		#Ename poo
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Xpos.bhvr
		part1	:mu_Heal_Orb.part
		pmagnet	T7
		##geom    Testing_Target
#		magnet	head
	End

	
	Event
		#Ename poo
		Type	Local
	
		At	Y45
		bhvr	:mu_Heal_Xneg.bhvr
		part1	:mu_Heal_Orb.part
		pmagnet	T8
		##geom    Testing_Target
#		magnet	head
	End

end


