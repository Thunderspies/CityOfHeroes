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

LifeSpan	30

########################################################

#
#Condition
#
#	On 	time
#	Time 	0 
#	Event
#		
#		Type	PositOnly
#		At	Hips
#		Sound	elecballhit2 100 30 .5
#				
#	End
#End

Condition

	On 	time
	Time 	0

	Event
		Ename	Grounder
		Type	start  
		At	chest
#		bhvr	:mu_StaticArcTarget.bhvr
		##geom    Testing_Target
	end

	Event
		Ename	Arc1
		Type	start  
		At	origin ##chest
		bhvr	:mu_StaticArc_Start.bhvr
		##geom    Testing_Target
		magnet	Grounder
		LookAt	Grounder
	end

	Event
		Ename	Arc2
		Type	local  
		At	Arc1
		bhvr	:mu_StaticArc_090_1.bhvr
		##geom    Testing_Target
	end
	
	Event
		Ename	Arc3
		Type	local  
		At	Arc1
		bhvr	:mu_StaticArc_090_2.bhvr
		##geom    Testing_Target
	end
	
	Event
		Ename	Arc4
		Type	local  
		At	Arc1
		bhvr	:mu_StaticArc_090_3.bhvr
		##geom    Testing_Target
	end
	
	Event
		Ename	Arc5
		Type	local  
		At	Arc1
		bhvr	:mu_StaticArc_end.bhvr
		##geom    Testing_Target
	end


########	
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
		part1	:mu_Heal_Branches4.part
		lifespan 15
		###geom    Testing_Target
		Pmagnet	Root
#		pother	arc1
	end

########	
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
		part1	:mu_Heal_Branches3.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc2
	end

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
		part1	:mu_Heal_Branches2.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc2
	end

########	
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
		part1	:mu_Heal_Branches.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc2
	end


End


Condition

End