#########################################################
##	
##

FxInfo
LifeSpan  150

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################


Condition
	Event
		Type Local
		At LarmL
		Sound scorpionattack1b 100 100 .99
	End
End



Condition
	On 	Time
	Time 	0

	Event
		ename	CANNON
		Type	Local 
		At	LarmL
#		geom    Testing_Target		
		BhvrOverride
			PositionOffset		-1.8 -.08 -.5 
		
		End
	End

end

Condition
	On 	Time
	Time 	13

	Event	
		ename	Charge
		Type	local
		At	cannon	
#		geom    Testing_Target
		bhvr	:BS_Tail_chargeball.bhvr
		part	:BS_Tail_chargeball.part
		part	:BS_Tail_chargeflare.part
		lifespan 55
	end

end

Condition
	On 	Time
	Time 	0

#	Event	
#		ename	Charge
#		Type	local
#		At	cannon	
##		geom    Testing_Target
#		bhvr	:BS_Tail_chargeball.bhvr
#		part	:BS_Tail_chargeball.part
#		part	:BS_Tail_chargeflare.part
#		lifespan 55
#	end
	Event	
		ename	smoke
		Type	local
		At	cannon	
#		part	:BS_Tail_DischargeSmoke.part
		part	:BS_Tail_DistSmoke.part
		part	:BS_Tail_DistSmoke2.part
		lifespan 53
	end
	Event	
		ename	smoke2
		Type	local
		At	cannon	
#		part	:BS_Tail_DischargeSmoke.part
		part	:BS_Tail_DistSmoke.part
		part	:BS_Tail_DistSmoke2.part
		lifespan 55
	end
	Event	
		ename	disChargeSpot
		Type	local
		At	waist	
#		geom    Testing_Target
		BhvrOverride
			PositionOffset		-3 2.2 3.3
		End
	end
	Event	
		ename	disCharge
		Type	positonly
		At	disChargeSpot	
#		geom    Testing_Target
		lookat	T_Chest
	end

	Event	
		ename	jointsmoke
		Type	local
		At	cannon	
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 120
	end



end

Condition
	On 	time
	Time 	48




	Event
		ENAME   prime
		Type	start
		At	disCharge
		bhvr	:BS_Tail_Blast.bhvr
#		geom    Testing_Target	
		Part1	:BS_Tail_BallTailBig.part
		Part1	:BS_Tail_BallTailBig.part
		Part1	:BS_Tail_ShotBallGlow.part
		Part1	:BS_Tail_SteamTrail.part
		magnet	t_chest
		BhvrOverride
			initialvelocity		0 .1 0
			TrackRate		4
			scale			 2 2 2
		End
		
		lifespan 60
	End
	Event
		Type	local
		At	prime

		Part	:BS_Armblast_embers.part
	end
	Event
		Type	start
		At	prime

		Part1	:BS_Tail_ShotRing.part
		Part1	:BS_Tail_ShotRing.part
	end


	Event
		ENAME   screw
		Type	local
		At	prime

		bhvr	:BS_Tail_Twist.bhvr
	end

	Event
		ENAME   ballcone
		Type	local
		At	screw

		Part	:BS_Armblast_cone.part
		
		bhvroverride
			PyrRotate	-90 0 0
		end
	end



End	
	



Condition
	On 	primehit
	
	Event
		EName 	Prime
		Type	Destroy
	End


end


End				