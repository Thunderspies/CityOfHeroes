#########################################################
##	forceBubble
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

LifeSpan	40

########################################################


Condition

	On 	time
	Time 	0

	Event
		Ename	base
		Type	local 
	
		Bhvr	:TargetOffset.bhvr
		At	root
		Lifespan 40
	end

end

Condition
	On 	Time
	Time 	0 #18

	Event
		Type	posit 
		Ename	Aim
	
		At	chest
		bhvr	:mu_Elec_ArcAim1.bhvr
		
		#Geom    Testing_Target
		lifespan 40
		LookAt	base
	End
	
		
	Event
		Type	local 
		Ename	RotSpinner
	
		At	Aim
		bhvr	:mu_Elec_ArcRotSpinner1.bhvr
		
		#Geom    Testing_Target
		lifespan 40
	End


END




Condition

	On 	time
	Time 	3

	Event
		Ename	Arc1
		Type	start  
		At	RotSpinner
		bhvr	:Mu_BlastArc_Start.bhvr
		#geom    Testing_Target
		lifespan 40
	end

	Event
		Ename	Arc2
		Type	local  
		At	Arc1
		bhvr	:Mu_BlastArc_1.bhvr
		#geom    Testing_Target
	end
	
	Event
		Ename	Arc3
		Type	local  
		At	Arc1
		bhvr	:Mu_BlastArc_2.bhvr
		#geom    Testing_Target
	end
	
	Event
		Ename	Arc4
		Type	local  
		At	Arc1
		bhvr	:Mu_BlastArc_3.bhvr
		#geom    Testing_Target
	end

	Event
		Ename	Arc5
		Type	local  
		At	Arc1
		bhvr	:Mu_BlastArc_4.bhvr
		#geom    Testing_Target
	end
#
#	
#	Event
#		Ename	Arc6
#		Type	local  
#		At	root
#		bhvr	:Arc_end.bhvr
#		#geom    Testing_Target
#	end


#################

	Event
		Ename	walker1
		Type	PositOnly    
		At	arc1
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end

	Event
		Type	local  
		At	walker1
#		part1	:Elec_ArcLink1.part
#		part2	:Elec_ArcLink1.part
#		part3	:Elec_ArcLink1.part
		part4	:Elec_ArcLink1.part
		part5	:Elec_ArcLinkCore1.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end

########	
end


Condition
	On 	Time
	Time 	4


	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end

	Event
		Type	local  
		At	walker2
#		part1	:Elec_ArcLink1.part
#		part2	:Elec_ArcLink1.part
#		part3	:Elec_ArcLink1.part
		part4	:Elec_ArcLink1.part
		part5	:Elec_ArcLinkCore1.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end
end


Condition
	On 	Time
	Time 	5
########	
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end

	Event
		Type	local  
		At	walker3
#		part1	:Elec_ArcLink1.part
#		part2	:Elec_ArcLink1.part
#		part3	:Elec_ArcLink1.part
		part4	:Elec_ArcLink1.part
		part5	:Elec_ArcLinkCore1.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end

########	
end


Condition
	On 	Time
	Time 	6
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end

	Event
		Type	local  
		At	walker4
#		part1	:Elec_ArcLink1.part
#		part2	:Elec_ArcLink1.part
#		part3	:Elec_ArcLink1.part
		part4	:Elec_ArcLink1.part
		part5	:Elec_ArcLinkCore1.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end

########	
end


Condition
	On 	Time
	Time 	7
	Event
		Ename	walker5
		Type	PositOnly    
		At	Arc5
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	base
	end

	Event
		Type	local  
		At	walker5
#		part1	:Elec_ArcLink1.part
#		part2	:Elec_ArcLink1.part
#		part3	:Elec_ArcLink1.part
		part4	:Elec_ArcLink1.part
		part5	:Elec_ArcLinkCore1.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end



End


