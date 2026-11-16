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
		At C_Hind_LLegR
		Sound scorpionattack3 100 100 .99
	End
End


Condition
	On 	Time
	Time 	0

	Event
		ename	CANNON
		Type	Local 
		At	C_Hind_LLegR
#		geom    Testing_Target		
		BhvrOverride
			PositionOffset		0 0 -1.5
		End
	End

end



Condition
	On 	Time
	Time 	0

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
	Event	
		ename	smoke
		Type	local
		At	cannon	
#		part	:BS_Tail_DischargeSmoke.part
		part	:BS_Tail_DistSmoke.part
		part	:BS_Tail_DistSmoke2.part
		lifespan 36
	end
	Event	
		ename	smoke2
		Type	local
		At	cannon	
#		part	:BS_Tail_DischargeSmoke.part
		part	:BS_Tail_DistSmoke.part
		part	:BS_Tail_DistSmoke2.part
		lifespan 39
	end
#	Event	
#		ename	disCharge2
#		Type	local
#		At	root	
#		geom    Testing_Target
#		BhvrOverride
#			PositionOffset		0 5.5 3.6
#		End
#	end
	Event	
		ename	disChargeSpot
		Type	local
		At	waist	
#		geom    Testing_Target
		BhvrOverride
			PositionOffset		0 3.2 1.6
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
		lifespan 120
	end
	Event	
		ename	jointsmoke
		Type	local
		At	C_Hind_LLegR	
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 85
	end
	Event	
		ename	jointsmoke
		Type	local
		At	C_Hind_ULegR
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 80
	end
	Event	
		ename	jointsmoke
		Type	local
		At	C_Hind_ToeL	
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 75
	end
	Event	
		ename	jointsmoke
		Type	local
		At	C_Hind_FootL	
		part	:BS_Tail_JointSmoke.part
		part	:BS_Tail_JointSmoke.part
		lifespan 70
	end


end

Condition
	On 	time
	Time 	33




	Event
		ENAME   prime
		Type	start
		At	disCharge
		bhvr	:BS_Tail_Blast.bhvr
#		geom    Testing_Target	
		Part1	:BS_Tail_BallTailBig.part
		Part1	:BS_Tail_ShotBallGlow.part
		Part1	:BS_Tail_SteamTrail.part
		magnet	t_chest
		BhvrOverride
			initialvelocity		0 .1 0
		End
		
		lifespan 60
	End


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
		Type	local
		At	screw

		bhvr	:BS_Tail_TwistOff.bhvr
		Part1	:BS_Tail_BallTwist.part
		Part1	:BS_Tail_BallTwist.part
		lifespan 60
	end


	Event
		Type	local
		At	screw

		bhvr	:BS_Tail_TwistOff2.bhvr
		Part1	:BS_Tail_BallTwist.part
		Part1	:BS_Tail_BallTwist.part
		lifespan 60
	end

End	
	



Condition
	On 	primehit
	
	Event
		EName 	Prime
		Type	Destroy
	End


	Event	
		Type	start
		At	T_Chest	
		childfx :BS_Tail_blast_Shrapnel.fx
	end

	Event	
		Type	start
		At	T_Chest	
		childfx :BS_Tail_blast_Shrapnel.fx
	end

	Event	
		Type	start
		At	T_Chest	
		childfx :BS_Tail_blast_Shrapnel.fx
	end

	Event	
		Type	start
		At	T_Chest	
		childfx :BS_Tail_blast_Shrapnel.fx
	end





end


End				