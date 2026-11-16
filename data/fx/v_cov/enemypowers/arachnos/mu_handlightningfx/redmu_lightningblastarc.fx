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
	
		At	wepL
		Lifespan 40
	end

end

Condition

	On 	cycle
	Time 	2
	Event
		Type	Start 
		Ename	Charger1
	
		At	wepL
		bhvr	V_COV\EnemyPowers\Arachnos\mu_Elec_BlastChargers1.bhvr
		Magnet	Chest
		Lifespan 2
	End
#
#	Event
#		Type	start 
#		ename	ChargeArcs
#		At	wepL
#		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BlastChargers1.part
#		Lifespan 2
#		pmagnet	root
#	end
#	Event
#		Type	Start 
#		Ename	Charger2
#	
#		At	wepL
#		bhvr	V_COV\EnemyPowers\Arachnos\mu_Elec_BlastChargers1.bhvr
#		Magnet	Chest
#		Lifespan 2
#	End
#
#	Event
#		Type	start 
#		ename	ChargeArcs
#		At	wepL
#		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BlastChargers1.part
#		Lifespan 2
#		pmagnet	root
#	end
#
End

Condition

	On 	time
	Time 	10

	Event
	Ename	Charger1
	Type	destroy 
	
	end
		Event
	Ename	ChargeArcs
	Type	destroy 
	
	end
end



Condition
	On 	Time
	Time 	0 #18

	Event
		Type	posit 
		Ename	Aim
	
		At	wepL
		bhvr	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcAim1.bhvr
		
		#Geom    Testing_Target
		lifespan 40
		LookAt	T_chest
	End
	
		
	Event
		Type	local 
		Ename	RotSpinner
	
		At	wepL
		bhvr	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcRotSpinner1.bhvr
		
		#Geom    Testing_Target
		lifespan 40
	End


END




Condition

	On 	time
	Time 	16

	Event
		Ename	Arc1
		Type	start  
		At	RotSpinner
		bhvr	V_COV\EnemyPowers\Arachnos\Mu_BlastArc_Start.bhvr
		#geom    Testing_Target
		lifespan 40
	end

	Event
		Ename	Arc2
		Type	local  
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\Mu_BlastArc_1.bhvr
		#geom    Testing_Target
	end
	
	Event
		Ename	Arc3
		Type	local  
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\Mu_BlastArc_2.bhvr
		#geom    Testing_Target
	end
	
	Event
		Ename	Arc4
		Type	local  
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\Mu_BlastArc_3.bhvr
		#geom    Testing_Target
	end

	Event
		Ename	Arc5
		Type	local  
		At	Arc1
		bhvr	V_COV\EnemyPowers\Arachnos\Mu_BlastArc_4.bhvr
		#geom    Testing_Target
	end
#
#	
#	Event
#		Ename	Arc6
#		Type	local  
#		At	root
#		bhvr	V_COV\EnemyPowers\Arachnos\Arc_end.bhvr
#		#geom    Testing_Target
#	end


#################

	Event
		Ename	walker1
		Type	PositOnly    
		At	arc1
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc2
	end

	Event
		Type	local  
		At	walker1
		part4	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkSubtle.part
		part5	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkCoreSubtle.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end

########	
end


Condition
	On 	Time
	Time 	17


	Event
		Ename	walker2
		Type	PositOnly    
		At	Arc2
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc3
	end

	Event
		Type	local  
		At	walker2
		part4	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkSubtle.part
		part5	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkCoreSubtle.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end
end


Condition
	On 	Time
	Time 	18
########	
	Event
		Ename	walker3
		Type	PositOnly    
		At	Arc3
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc4
	end

	Event
		Type	local  
		At	walker3
		part4	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkSubtle.part
		part5	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkCoreSubtle.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end

########	
end


Condition
	On 	Time
	Time 	19
	Event
		Ename	walker4
		Type	PositOnly    
		At	Arc4
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	Arc5
	end

	Event
		Type	local  
		At	walker4
		part4	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkSubtle.part
		part5	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkCoreSubtle.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end

########	
end


Condition
	On 	Time
	Time 	20
	Event
		Ename	walker5
		Type	PositOnly    
		At	Arc5
		bhvr	V_COV\EnemyPowers\Arachnos\mu_StaticArcWalker1.bhvr
		lifespan 5
		##geom    Testing_Target
		magnet	t_chest
	end

	Event
		Type	local  
		At	walker5
		part4	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkSubtle.part
		part5	V_COV\EnemyPowers\Arachnos\mu_Elec_ArcLinkCoreSubtle.part
		Pmagnet T_Hips
		#Geom    Testing_Target
		lifespan 30
	end



End


