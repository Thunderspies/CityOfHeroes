#########################################################
##	
##
##
##The Black Scorpion’s flaming tail effect fades over time so that the fire that comes out of the cannons become completely invisible over the span of about a minute, then gradually reappear a minute later. The additive fire is actually fading to full black and then back to the correct color, alpha works fine. I attempted to counteract this bug by setting a color keyframe at frame 6000 in the .part file but this did not make a difference. I haven’t seen any/many effects that last this long without being reapplied and the bug is so subtle that it’s easy to miss.
##
## 
##
##The offending particle: V_COV\EnemyPowers\ArchVillains\BlackScorpion\BS_WepSmoke.part
##
##BS_WeaponGlow.fx and BS_WeaponGlow_Tail.fx are the FX that call to this particle, but you need to have a tail entity attached to display the tail effect. Summoning or Benpc signature_black_scorpion is the easiest way to see it
##
##
##






FxInfo
#LifeSpan  150



##################################
Condition
	On 	Time
	Time 	0

	Event
		ename	CANNON
		Type	Local 
		At	Hind_LLegR
#		geom    Testing_Target		
		BhvrOverride
			PositionOffset		0 0 -1.5
		End
	End
#
#	Event
#		ename	HANDCANNON
#		Type	Local 
#		At	LarmL
##		geom    Testing_Target		
#		BhvrOverride
#			PositionOffset		-1.4 -.08 -.5 
#		End
#	End

end



Condition
	On 	Time
	Time 	0

	Event	
		ename	Charge ##projected energy
		Type	local
		At	cannon	
#		geom    Testing_Target
		bhvroverride
			PyrRotate		0 0 90 
			PositionOffset		.07 .051 .2
			Scale			.7 .7 1.2
		end
		part	:BS_Tail_glow.part
	end

	Event	
		ename	Charge2	#barrel energy
		Type	local
		At	cannon	
		bhvroverride
			PyrRotate		90 0 0  
			PositionOffset		0 .1 .2
			Scale			.4 .8 1.5
		end
		part	:BS_Tail_glow.part
	end



end

#
#Condition
#	On 	Time
#	Time 	0
#
#	Event	
#		ename	HCharge  ##projected energy
#		Type	local
#		At	handcannon	
#		bhvroverride
#			PositionOffset		-.2 .22 0
#			Scale			1 1.4 .8
#		end
#		part	:BS_Tail_glow.part
#	end
#
#	Event	
#		ename	HCharge2 	#barrel energy
#		Type	local
#		At	handcannon	
#		bhvroverride
#			PyrRotate		 0 0 90 
#			PositionOffset		0 .1 .051
#			Scale			1 .6 1
#		end
#		part	:BS_Tail_glow.part
#	end
#
#
#
#
#end

Condition
	On 	Time
	Time 	0
	#################### these particles fade to black (invis) over time
	Event	
		ename	Charge3
		Type	local
		At	cannon	
		part	:BS_WepSmoke.part
		part	:BS_WepSmoke.part
	end

#	Event	
#		ename	HCharge3
#		Type	local
#		At	handcannon
#		bhvroverride
#			PositionOffset		-.3 0 0
#		end	
#		part	:BS_WepSmoke.part
#		part	:BS_WepSmoke.part
##		lifespan 600
#	end
end


Condition
	On 	triggerbits
	Triggerbits	death

	
	Event
		EName 	cannon
		Type	destroy
	
	End
	Event
		EName 	handcannon
		Type	destroy
	
	End

end


End				