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

LifeSpan	11

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
		Type	local  
		At	Root

		BhvrOverride
			PositionOffset	9 0 0 
		End

		bhvr	:mu_StaticArcTarget.bhvr
		#geom    Testing_Target
	end

	Event
		Ename	Grounder2
		Type	local  
		At	Root

		BhvrOverride
			PositionOffset	-9 0 0 
		End

		bhvr	:mu_StaticArcTarget.bhvr
		#geom    Testing_Target
	end

	Event
		Ename	Arc1
		Type	start  
		At	C_HandR
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
		part1	:mu_EMP_BranchesPurple.part
		part1	:mu_EMP_Branches2Purple.part
		Part	:mu_EMP_BranchesWhite.part
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
		part1	:mu_EMP_BranchesPurple.part
		part1	:mu_EMP_Branches2Purple.part
		Part	:mu_EMP_BranchesWhite.part
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
		part1	:mu_EMP_BranchesPurple.part
		part1	:mu_EMP_Branches2Purple.part
		Part	:mu_EMP_BranchesWhite.part
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
		part1	:mu_EMP_BranchesPurple.part
		part1	:mu_EMP_Branches2Purple.part
		Part	:mu_EMP_BranchesWhite.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc2
	end



	Event
		Type	startpositonly  
		At	grounder
		part1	:mu_EMP_OrbSpotsPurple.part
		part1	:mu_EMP_OrbSpotsFrontPurple.part
		Part	:mu_EMP_BranchesWhite.part
		lifespan 1
		##geom    Testing_Target
	end


##############################################################################


	Event
		Ename	Arc12
		Type	start  
		At	C_HandL
		bhvr	:mu_StaticArc_Start.bhvr
		##geom    Testing_Target
		magnet	Grounder2
		LookAt	Grounder2
	end

	Event
		Ename	Arc22
		Type	local  
		At	Arc12
		bhvr	:mu_StaticArc_090_1.bhvr
		##geom    Testing_Target
	end
	
	Event
		Ename	Arc32
		Type	local  
		At	Arc12
		bhvr	:mu_StaticArc_090_2.bhvr
		##geom    Testing_Target
	end
	
	Event
		Ename	Arc42
		Type	local  
		At	Arc12
		bhvr	:mu_StaticArc_090_3.bhvr
		##geom    Testing_Target
	end
	
	Event
		Ename	Arc52
		Type	local  
		At	Arc12
		bhvr	:mu_StaticArc_end.bhvr
		##geom    Testing_Target
	end


########	
	Event
		Ename	walker12
		Type	PositOnly    
		At	arc12
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc22
	end

	Event
		Type	local  
		At	walker12
		part1	:mu_EMP_BranchesPurple.part
		part1	:mu_EMP_Branches2Purple.part
		Part	:mu_EMP_BranchesWhite.part
		lifespan 15
		###geom    Testing_Target
		Pmagnet	Root
#		pother	arc12
	end

########	
	Event
		Ename	walker22
		Type	PositOnly    
		At	Arc22
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc32
	end

	Event
		Type	local  
		At	walker22
		part1	:mu_EMP_BranchesPurple.part
		part1	:mu_EMP_Branches2Purple.part
		Part	:mu_EMP_BranchesWhite.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc22
	end

########	
	Event
		Ename	walker32
		Type	PositOnly    
		At	Arc32
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc42
	end

	Event
		Type	local  
		At	walker32
		part1	:mu_EMP_BranchesPurple.part
		part1	:mu_EMP_Branches2Purple.part
		Part	:mu_EMP_BranchesWhite.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc22
	end

########	
	Event
		Ename	walker42
		Type	PositOnly    
		At	Arc42
		bhvr	:mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc52
	end

	Event
		Type	local  
		At	walker42
		part1	:mu_EMP_BranchesPurple.part
		part1	:mu_EMP_Branches2Purple.part
		Part	:mu_EMP_BranchesWhite.part
		lifespan 15
		##geom    Testing_Target
		Pmagnet	Root
#		pother	arc22
	end



	Event
		Type	startpositonly  
		At	grounder2
		part1	:mu_EMP_OrbSpotsPurple.part
		part1	:mu_EMP_OrbSpotsFrontPurple.part
		Part	:mu_EMP_BranchesWhite.part
		lifespan 1
		##geom    Testing_Target
	end


End
